
#include "DT5202.h"



int main(int argc, char **argv){
  DT5202 *board = new DT5202();
  
  board->Init("192.168.2.240","");

  

  
  board->Close();
}
