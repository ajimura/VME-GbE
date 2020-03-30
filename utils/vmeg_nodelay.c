#define DEFAULT_IP "172.16.205.120" //vmeg0
#define DEFAULT_PORT 4660

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "rbcp_lib.h"

int main(int argc, char* argv[]){

  char* sitcpIpAddr;
  unsigned int sitcpPort;

  int st;

  unsigned char data[2048];
  unsigned int addr;

  if(argc != 3){
    sitcpIpAddr = DEFAULT_IP;
      sitcpPort   = DEFAULT_PORT;
  }else{
    sitcpIpAddr = argv[1];
    sitcpPort   = atoi(argv[2]);
  }

  if (rbcp_ini(sitcpIpAddr, sitcpPort)<0) exit(-1);

  addr=0xffffff10;
  data[0]=0x04;

  st=rbcp_wr(addr,data,1);
  if (st<0) printf("Error...\n");

  rbcp_fin();
  exit(0);
}
