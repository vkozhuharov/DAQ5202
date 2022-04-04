#include "DT5202.h"
#include <iostream>

DT5202::DT5202(){
  handle = 0;
  CtrlSocket = f_socket_invalid;
  DataSocket = f_socket_invalid;
  ethRxBuff = NULL;
}

int DT5202::Init(const char* addr,const char *cfg){
  
  std::cout << "Board with IP " << addr << "  initialized " << std::endl;
  
  return 0;
}
