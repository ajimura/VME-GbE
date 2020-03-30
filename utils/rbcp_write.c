#define DEFAULT_IP "172.16.207.238"
//#define DEFAULT_IP "192.168.208.85"
#define DEFAULT_PORT 4660

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "rbcp_lib.h"

int main(int argc, char* argv[]){

  char* sitcpIpAddr;
  unsigned int sitcpPort;
  char line[100];

  int st;

  unsigned char data;
  unsigned int addr;
  unsigned int input;

  if(argc != 3){
    sitcpIpAddr = DEFAULT_IP;
      sitcpPort   = DEFAULT_PORT;
  }else{
    sitcpIpAddr = argv[1];
    sitcpPort   = atoi(argv[2]);
  }

  if (rbcp_ini(sitcpIpAddr, sitcpPort)<0) exit(-1);

  printf("Address ? ");
  fgets(line,100,stdin);
  sscanf(line,"%x",&addr);
  printf("Data ? ");
  fgets(line,100,stdin);
  sscanf(line,"%x",&input);
  data=(unsigned char)input;

  st=rbcp_wr(addr,&data,1);
  if (st<0) printf("Error...\n");

  rbcp_fin();
  exit(0);
}
