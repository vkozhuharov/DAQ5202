#include "DT5202.h"


DT5202::DT5202(){
  handle = 0;
  CtrlSocket = f_socket_invalid;
  DataSocket = f_socket_invalid;
  ethRxBuff = NULL;
}
