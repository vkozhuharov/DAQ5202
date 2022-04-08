#include "DT5202.h"
#include <iostream>
#include <sys/socket.h>	// send receive
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <stdio.h>


#define FERS_LibMsg printf


DT5202::DT5202(){
  handle = 0;
  CtrlSocket = f_socket_invalid;
  DataSocket = f_socket_invalid;
  ethRxBuff = NULL;
}

int DT5202::Init(const char* addr,const char *cfg){
  int res;
  uint32_t data=0;
  
  res = LLeth_OpenDevice(addr);
  std::cout << "Board with IP " << addr << "  initialized " << std::endl;

  res = LLeth_ReadRegister(a_fw_rev, &data);
  printf("\tFirmware revision: %d\n",data);
  res = LLeth_ReadRegister(a_channel_mask_0, &data);
  printf("\tChannel mask: %x \n",data);

  
  return 0;
}


f_socket_t DT5202::LLeth_ConnectToSocket(const char *board_ip_addr, char *port)
{
	f_socket_t ConnectSocket = f_socket_invalid;
	struct addrinfo *result = NULL, *ptr = NULL, hints;
	unsigned long ul = 1;
	int iResult, connect_fail = 0;

#ifdef _WIN32
	// Initialize Winsock
	WSADATA wsaData;
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		if (ENABLE_FERSLIB_LOGMSG) FERS_LibMsg("WSAStartup failed with error: %d\n", iResult);
		return 1;
	}
#endif

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	// Resolve the server address and port
	iResult = getaddrinfo(board_ip_addr, port, &hints, &result);
	if (iResult != 0) {
		if (ENABLE_FERSLIB_LOGMSG) FERS_LibMsg("LCSm", "getaddrinfo failed with error: %d\n", iResult);
#ifdef _WIN32
		WSACleanup();
#endif
		return f_socket_invalid;
	}

	// Attempt to connect to an address until one succeeds
	for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {

		// Create a SOCKET for connecting to server
		ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
		if (ConnectSocket == f_socket_invalid) {
			if (ENABLE_FERSLIB_LOGMSG) FERS_LibMsg("socket failed with error: %ld\n", f_socket_errno); // WSAGetLastError());
#ifdef _WIN32
			WSACleanup();
#endif
			return f_socket_invalid;
		}

		// Connect to server.
		/*
		iResult = connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
		if (iResult == SOCKET_ERROR) {
			closesocket(ConnectSocket);
			ConnectSocket = INVALID_SOCKET;
			continue;
		}*/
#ifdef _WIN32
		ioctlsocket(ConnectSocket, FIONBIO, &ul); //set as non-blocking
#else
		ioctl(ConnectSocket, FIONBIO, &ul);
#endif
		if (connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen) == -1) {
			fd_set set;
			int error = -1;
			unsigned int len = sizeof(int);
			struct timeval tm;

			tm.tv_sec = FERS_CONNECT_TIMEOUT;	// set timeout = 3s
			tm.tv_usec = 0;
			FD_ZERO(&set);
			FD_SET(ConnectSocket, &set);
			if (select((int)(ConnectSocket + 1), NULL, &set, NULL, &tm) > 0) {
				getsockopt(ConnectSocket, SOL_SOCKET, SO_ERROR, (char *)&error, &len);
				if (error == 0)	connect_fail = 0;
				else connect_fail = 1;
			} else {
				connect_fail = 1;
			}
		}
		ul = 0;
#ifdef _WIN32
		ioctlsocket(ConnectSocket, FIONBIO, &ul); //set as blocking
#else
		ioctl(ConnectSocket, FIONBIO, &ul);
#endif

		if (connect_fail) {
			f_socket_close(ConnectSocket);
			ConnectSocket = f_socket_invalid;
			continue;
		}
		break;
	}
	freeaddrinfo(result);

	if (ConnectSocket == f_socket_invalid) {
		if (ENABLE_FERSLIB_LOGMSG) FERS_LibMsg("Unable to connect to server!\n");
#ifdef _WIN32
		WSACleanup();
#endif
		return f_socket_invalid;
	} else return ConnectSocket;
}


// -----------------------------------------------------------------------------------
// R/W Reg via I2C (old version, slow... don't use it)
// -----------------------------------------------------------------------------------
int DT5202::LLeth_WriteReg_i2c( uint32_t address, uint32_t data)
{
	uint32_t res;
	char sendbuf[32];
	int iResult;
	f_socket_t sck = CtrlSocket;

	sendbuf[0] = 'A';
	sendbuf[1] = 'B';
	sendbuf[2] = 'B';
	sendbuf[3] = 'A';
	memcpy(&sendbuf[4], &address, 4);
	memcpy(&sendbuf[8], &data, 4);
	iResult = send(sck, sendbuf, 12, 0);
	if (iResult == f_socket_error) {
		if (ENABLE_FERSLIB_LOGMSG)FERS_LibMsg("WriteReg_i2c, send with error: %d\n", f_socket_errno); 
#ifdef _WIN32
		WSACleanup();
#endif
		return -1;
	}
	iResult = recv(sck, sendbuf, 4, 0);
	if (iResult == f_socket_error) {
		if (ENABLE_FERSLIB_LOGMSG) FERS_LibMsg("WriteReg_i2c, recv failed with error: %d\n", f_socket_errno); 
#ifdef _WIN32
		WSACleanup();
#endif
		return -1;
	}
	res = *((uint32_t *)sendbuf);
	if (res == 0) return -2;
	else return 0;
}

int DT5202::LLeth_ReadReg_i2c(uint32_t address, uint32_t *data)
{
	char sendbuf[32];
	int iResult;
	f_socket_t sck = CtrlSocket;
	
	sendbuf[0] = 'A';
	sendbuf[1] = 'B';
	sendbuf[2] = 'B';
	sendbuf[3] = 'C';
	memcpy(&sendbuf[4], &address, 4);
	iResult = send(sck, sendbuf, 8, 0);
	if (iResult == f_socket_error) {
		if (ENABLE_FERSLIB_LOGMSG) FERS_LibMsg("ReadReg_i2c, send failed with error: %d\n", f_socket_errno); 
		return -1;
	}
	iResult = recv(sck, sendbuf, 4, 0);
	if (iResult < 4)
		if (ENABLE_FERSLIB_LOGMSG) FERS_LibMsg("Incomplete read reg\n");
	if (iResult == f_socket_error) {
		if (ENABLE_FERSLIB_LOGMSG) FERS_LibMsg("ReadReg_i2c, recv failed with error: %d\n", f_socket_errno); // WSAGetLastError());

		return -1;
	}
	*data = *((uint32_t *)sendbuf);
	return 0;
}

int DT5202::LLeth_WriteMem(uint32_t address, char *data, uint16_t size)
{
	uint32_t res;
	char *sendbuf = (char *) malloc(12+size);// [32];
	int iResult;
	f_socket_t sck = CtrlSocket;
	sendbuf[0] = 'A';
	sendbuf[1] = 'B';
	sendbuf[2] = 'B';
	sendbuf[3] = 'D';
	memcpy(&sendbuf[4], &address, 4);
	memcpy(&sendbuf[8], &size, 4);
	memcpy(&sendbuf[12], data, size);

	iResult = send(sck, sendbuf, 12+size, 0);
 	if (iResult == f_socket_error) {
		if (ENABLE_FERSLIB_LOGMSG) FERS_LibMsg("Write mem, send failed with error: %d\n", f_socket_errno); 

		free(sendbuf);
		return -1;
	}

	iResult = recv(sck, sendbuf, 4, 0);
	if (iResult == f_socket_error) {
		if (ENABLE_FERSLIB_LOGMSG) FERS_LibMsg("Write mem, recv failed with error: %d\n", f_socket_errno); 


		free(sendbuf);
		return -1;
	}

	res = *((uint32_t *)sendbuf);
	free(sendbuf);

	if (res != 0) return -2;
	else return 0;
}

int DT5202::LLeth_ReadMem(uint32_t address, char *data, uint16_t size)
{
	char sendbuf[32];
	int iResult;
	f_socket_t sck = CtrlSocket;
	sendbuf[0] = 'A';
	sendbuf[1] = 'B';
	sendbuf[2] = 'B';
	sendbuf[3] = 'E';
	memcpy(&sendbuf[4], &address, 4);
	memcpy(&sendbuf[8], &size, 4);

	iResult = send(sck, sendbuf, 12, 0);
	if (iResult == f_socket_error) {
		if (ENABLE_FERSLIB_LOGMSG) FERS_LibMsg("Read mem, send failed with error: %d\n", f_socket_errno); 

		return -1;
	}

	iResult = recv(sck, data, size, 0);
	if (iResult == f_socket_error) {
		if (ENABLE_FERSLIB_LOGMSG) FERS_LibMsg("Read mem, recv failed with error: %d\n", f_socket_errno); 
		return -1;
	}
	return 0;
}

int DT5202::LLeth_WriteRegister(uint32_t address, uint32_t data) {
	return LLeth_WriteMem(address, (char *)&data, 4);
}

int DT5202::LLeth_ReadRegister( uint32_t address, uint32_t *data) {
	return LLeth_ReadMem(address, (char *)data, 4);
}


// -----------------------------------------------------------------------------------
// Read raw data
// -----------------------------------------------------------------------------------
int DT5202::LLeth_ReadData(char *buff, int size, int *nb) {
	int rnb, chunk;
	*nb = 0;
	//	const int r = trylock(ethRxMutex);
	//	if (r != 0)  // timeout = 10 ms
	//		return 0;
	rnb = (ethBuff_wp - ethBuff_rp) % RX_BUFF_SIZE;  // num of bytes currently available in ethRxBuff
	if (rnb > size) rnb = size;
	if (rnb > 0) {
		chunk = RX_BUFF_SIZE - ethBuff_rp;  // num of bytes from read pointer to end of buffer
		if (chunk >= rnb) {
			memcpy(buff, ethRxBuff + ethBuff_rp, rnb);
		} else {
			memcpy(buff, ethRxBuff + ethBuff_rp, chunk);
			memcpy(buff+chunk, ethRxBuff, rnb-chunk);
		}
		ethBuff_rp = (ethBuff_rp + rnb) % RX_BUFF_SIZE;
		*nb = rnb;
	}
	//	unlock(ethRxMutex);
	return 0;
}

int DT5202::LLeth_OpenDevice(const char *board_ip_addr){
  int started, ret;
  CtrlSocket = LLeth_ConnectToSocket(board_ip_addr, COMMAND_PORT);
  if ((CtrlSocket == f_socket_invalid) )  {
    return FERSLIB_ERR_COMMUNICATION;
  }
  
  ret = LLeth_WriteRegister(a_commands, CMD_ACQ_STOP);	// stop acquisition (in case it was still running)
  ret |= LLeth_WriteRegister(a_commands, CMD_CLEAR);		// clear data in the FPGA FIFOs

  return 0;
 
}


int DT5202::LLeth_CloseDevice()
{

  shutdown(CtrlSocket, SHUT_SEND);	
  if (CtrlSocket != f_socket_invalid) {
    f_socket_close(CtrlSocket);
  }

  return 0;
}


void DT5202::Close() {
  LLeth_CloseDevice();
}



