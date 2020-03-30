// test utility for VME-GbE
// Author: Shuhei Ajimura, RCNP, Osaka University

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "vmeg_lib.h"

int main(int argc, char *argv[]){
  char *IpAddr;
  int IpPort;
  int sock;
  char line[100];
  int loop;
  int length;
  int ret;
  unsigned short data16[65536];
  int i,j;
  unsigned int address;
  /* for time */
  struct timespec ts0, ts1;
  double t0,t1;
  int id=0;

  if (argc<3){
    printf("vme_test <ipaddr> <ipport>\n");
    exit(0);
  }

  IpAddr=argv[1];
  IpPort=atoi(argv[2]);

  if ((sock=VMEGopen(IpAddr, IpPort))<0) exit(1);

  printf("Loop? ");
  fgets(line,100,stdin);
  sscanf(line,"%d",&loop);

  printf("Length? ");
  fgets(line,100,stdin);
  sscanf(line,"%d",&length);

  address=0x001111fa;

  clock_gettime(CLOCK_MONOTONIC,&ts0);

  for(i=0;i<loop;i++){
    if ((ret=VMEGread_req(sock,A24|D16|FIX,address,data16,length,id))<0){
      printf("Error!\n");
      VMEGclose(sock);
      exit(-1);
    }
    if ((ret=VMEGread_req(sock,A24|D16|FIX,address,data16+length/2,length,id+1))<0){
      printf("Error!\n");
      VMEGclose(sock);
      exit(-1);
    }
    if ((ret=VMEGread_rcv(sock,A24|D16|FIX,address,data16,length,id))<0){
      printf("Error!\n");
      VMEGclose(sock);
      exit(-1);
    }
    if ((ret=VMEGread_rcv(sock,A24|D16|FIX,address,data16+length/2,length,id+1))<0){
      printf("Error!\n");
      VMEGclose(sock);
      exit(-1);
    }

    printf("Read data: ");
    for(j=0;j<length;j++)
      printf("%04x ",*(data16+j));
    printf("\n");

    id+=2;
  }

  clock_gettime(CLOCK_MONOTONIC,&ts1);
  t0=(ts0.tv_sec*1.)+(ts0.tv_nsec/1000000000.);
  t1=(ts1.tv_sec*1.)+(ts1.tv_nsec/1000000000.);

  printf("time: %lf %lf %lf\n",t1,t0,t1-t0);

  VMEGclose(sock);
  exit(0);
}
