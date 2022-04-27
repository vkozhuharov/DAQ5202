/*=================================================== */
/* The code is largely based on the Janus open source */
/* code from CAEN SpA - Front End Division            */

#ifndef _DT5202_H_
#define _DT5202_H_

#include <cstdint>
#include <cstddef>
#include <cstring>

#include "FERSlib.h"
#include "FERS_Registers.h"

#define Sleep(ms)				usleep((ms)*1000)


#define COMMAND_PORT		"9760"  // Slow Control Port (access to Registers)
#define STREAMING_PORT		"9000"	// Data RX Port (streaming)

#define ETH_BLK_SIZE  (1024)						// Max size of one packet in the recv 
#define RX_BUFF_SIZE  (1024*1024)					// Size of the local Rx buffer

// #define ENABLE_FERSLIB_LOGMSG 0
// #define FERS_CONNECT_TIMEOUT		3		// timeout connection in seconds


//#define FERS_LibMsg printf
#define SHUT_SEND	1
#define SHUT_BOTH	2


//Ethernet defines
typedef int							f_socket_t;			//!< Return type of socket(). On Linux socket() returns int.
#define f_socket_errno				errno				//!< On Linux socket-related functions set the error into errno variable.
#define f_socket_h_errno			h_errno				//!< On Linux network database operations like gethostbyname() set the error into h_errno variable.
#define f_socket_invalid			(-1)				//!< On Windows functions like accept() return INVALID_SOCKET in case of error. On Linux they return -1.
#define f_socket_error				(-1)				//!< On Windows functions like send() return SOCKET_ERROR in case of error. On Linux they return -1.
#define f_socket_close(f_sock)		close(f_sock)		//!< On Windows closesocket. On linux close>




class DT5202 {
 public:
  DT5202();
  void Init(){;};
  int Init(const char*, const char*);
  void Run(){;};
  void Close();

  int LLeth_OpenDevice(const char *board_ip_addr);
  int LLeth_CloseDevice();

  f_socket_t LLeth_ConnectToSocket(const char *board_ip_addr, char *port);
  int LLeth_WriteRegister(uint32_t address, uint32_t data);
  int LLeth_ReadRegister(uint32_t address, uint32_t *data);
  
  // -----------------------------------------------------------------------------------
  // According to CAEN:   R/W Reg via I2C (old version, slow... don't use it)
  // -----------------------------------------------------------------------------------
  int LLeth_ReadReg_i2c(uint32_t address, uint32_t *data);
  int LLeth_WriteReg_i2c(uint32_t address, uint32_t data);
  
  // -----------------------------------------------------------------------------------
  // According to CAEN:   R/W Reg via SPI (new version, faster than I2C)
  // -----------------------------------------------------------------------------------
  int LLeth_ReadMem(uint32_t address, char *data, uint16_t size);
  int LLeth_WriteMem(uint32_t address, char *data, uint16_t size);
  
  int LLeth_ReadData(char *buff, int size, int *nb);



  

 private:
  void *ptr;
  int handle;
  f_socket_t CtrlSocket;
  f_socket_t DataSocket;
  FERS_BoardInfo_t BoardInfo;
  char *ethRxBuff;
  uint32_t ethBuff_wp;
  uint32_t ethBuff_rp;
  
};



#endif
