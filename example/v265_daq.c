// test utility for VME-GbE
// Author: Shuhei Ajimura, RCNP, Osaka University

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include "vmeg_lib.h"

#define BASEADDR 0x00111100
#define ADDRDAT 0x00000008
#define ADDRCLR 0x00000002
#define ADDRSCR 0x00000000

int main(int argc, char *argv[]){
  char *IpAddr;
  int IpPort;
  int sock;
  char line[100];
  int loop;
  int length;
  int ret;
  unsigned short data16[128];
  int i,j;
  unsigned int address;
  /* for time */
  struct timespec ts0, ts1;
  double t0,t1;
  unsigned short scr;
  int waitcount;

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

  address=BASEADDR+ADDRCLR;
  if ((ret=VMEGread(sock,A24|D16,address,data16,2))<0){
    printf("Error!\n");
    VMEGclose(sock);
    exit(-1);
  }

  for(i=0;i<loop;i++){
    /* get current status */
    address=BASEADDR+ADDRSCR;
    if ((ret=VMEGread(sock,A24|D16,address,&scr,2))<0){
      printf("Error!\n");
      VMEGclose(sock);
      exit(-1);
    }
    waitcount=0;
    /* wait "not empty" */
    while((scr&0x8000)==0){
      address=BASEADDR+ADDRSCR;
      if ((ret=VMEGread(sock,A24|D16,address,&scr,2))<0){
	printf("Error!\n");
	VMEGclose(sock);
	exit(-1);
      }
      if ((waitcount++)>10000){
	printf("Timeout!\n");
	VMEGclose(sock);
	exit(-1);
      }
      usleep(200);
    }
    clock_gettime(CLOCK_MONOTONIC,&ts0);

    /* get data */
    address=BASEADDR+ADDRDAT;
    if ((ret=VMEGread_a24d16_fix(sock,address,data16,32))<0){
      printf("Error!\n");
      VMEGclose(sock);
      exit(-1);
    }

    clock_gettime(CLOCK_MONOTONIC,&ts1);
    t0=(ts0.tv_sec*1.)+(ts0.tv_nsec/1000000000.);
    t1=(ts1.tv_sec*1.)+(ts1.tv_nsec/1000000000.);
    printf("--- %d --------------------------------------\n",i);
    printf("time: %lf %lf %lf\n",t1,t0,t1-t0);

    /* print data */
    printf("Read data: ");
    for(j=0;j<16;j++)
      printf("%04x ",data16[j]);
    printf("\n");

    address=BASEADDR+ADDRCLR;
    if ((ret=VMEGread(sock,A24|D16,address,data16,2))<0){
      printf("Error!\n");
      VMEGclose(sock);
      exit(-1);
    }
  }


  VMEGclose(sock);
  exit(0);
}
