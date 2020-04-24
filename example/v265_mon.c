// test utility for VME-GbE
// Author: Shuhei Ajimura, RCNP, Osaka University

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include "vmeg_lib.h"

#define BASEADDR 0x00111100
#define ADDRCLR 0x00000002
#define ADDRSCR 0x00000000
#define ADDRDAT 0x00000008

int main(int argc, char *argv[]){
  char *IpAddr;
  int IpPort;
  int sock;
  char line[100];
  int loop;
  int length;
  int ret;
  unsigned short data16[128];
  int i;
  unsigned int address;
  /* for time */
  struct timespec ts0, ts1;
  double t0,t1;

  if (argc<3){
    printf("vme_test <ipaddr> <ipport>\n");
    exit(0);
  }

  IpAddr=argv[1];
  IpPort=atoi(argv[2]);

  if ((sock=VMEGopen(IpAddr, IpPort))<0) exit(1);

  address=BASEADDR+ADDRSCR;

  loop=0;
  while(1){
    if ((ret=VMEGread(sock,A24|D16,address,data16,2))<0){
      printf("Error!\n");
      VMEGclose(sock);
      exit(-1);
    }
    printf("%4d: %04x\n",loop,data16[0]);
    usleep(10000);
    loop++;
  }

  VMEGclose(sock);
  exit(0);
}
