/*=================================================== */
/* The code is largely based on the Janus open source */
/* code from CAEN SpA - Front End Division            */

#ifndef _DT5202_H_
#define _DT5202_H_




#define COMMAND_PORT		"9760"  // Slow Control Port (access to Registers)
#define STREAMING_PORT		"9000"	// Data RX Port (streaming)

//Ethernet defines
typedef int							f_socket_t;			//!< Return type of socket(). On Linux socket() returns int.
#define f_socket_errno				errno				//!< On Linux socket-related functions set the error into errno variable.
#define f_socket_h_errno			h_errno				//!< On Linux network database operations like gethostbyname() set the error into h_errno variable.
#define f_socket_invalid			(-1)				//!< On Windows functions like accept() return INVALID_SOCKET in case of error. On Linux they return -1.
#define f_socket_error				(-1)				//!< On Windows functions like send() return SOCKET_ERROR in case of error. On Linux they return -1.
#define f_socket_close(f_sock)		close(f_sock)		//!< On Windows closesocket. On linux close>




class DT5202 {
 public:
  DT5202(){handle=0};
  void Init(){;};

  


 private:
  void *ptr;
  int handle;
  f_socket_t CtrlSocket;
  f_socket_t DataSocket;
  char *ethRxBuff;
  uint32_t ethBuff_wp;
  uint32_t ethBuff_rp;
  
};



#endif
