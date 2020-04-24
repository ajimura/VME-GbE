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

  address=BASEADDR+ADDRCLR;

  clock_gettime(CLOCK_MONOTONIC,&ts0);

  if ((ret=VMEGread(sock,A24|D16,address,data16,2))<0){
    printf("Error!\n");
    VMEGclose(sock);
    exit(-1);
  }

  clock_gettime(CLOCK_MONOTONIC,&ts1);
  t0=(ts0.tv_sec*1.)+(ts0.tv_nsec/1000000000.);
  t1=(ts1.tv_sec*1.)+(ts1.tv_nsec/1000000000.);

  printf("time: %lf %lf %lf\n",t1,t0,t1-t0);

  VMEGclose(sock);
  exit(0);
}
