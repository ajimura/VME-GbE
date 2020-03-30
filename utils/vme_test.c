// test utility for VME-GbE
// Author: Shuhei Ajimura, RCNP, Osaka University

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "vmeg_lib.h"

struct command_info{
  unsigned int rw; /* r=0, w=1 */
  unsigned int alength; /* 2,3,4 */
  unsigned int dlength; /* 1,2,4 */
  unsigned int address;
  unsigned int data;
};

int anal_command(char *, struct command_info *);

int main(int argc, char *argv[]){
  char *IpAddr;
  int IpPort;
  int sock;
  char line[100];
  struct command_info cominfo;
  int ret;
  unsigned char data8;
  unsigned short data16;
  unsigned int data32;
  unsigned short mode;

  if (argc<3){
    printf("vme_test <ipaddr> <ipport>\n");
    exit(0);
  }

  IpAddr=argv[1];
  IpPort=atoi(argv[2]);

  if ((sock=VMEGopen(IpAddr, IpPort))<0) exit(1);

  printf("Input VME command: <r/w/q> <a16/a24/a32> <d8/d16/d32> <address> [<write data>]\n");
  fgets(line,100,stdin);
  line[strlen(line)-1]=' ';
  while(line[0]!='q'){
    if (cominfo.alength==2) mode=A16;
    if (cominfo.alength==3) mode=A24;
    if (cominfo.alength==4) mode=A32;
    if (anal_command(line,&cominfo)==0){
      if (cominfo.rw==0){
	if (cominfo.dlength==1){
	  mode|=D8;
	  ret=VMEGread(sock,mode,cominfo.address,&data8,1);
	}else if (cominfo.dlength==2){
	  mode|=D16;
	  ret=VMEGread(sock,mode,cominfo.address,&data16,2);
	}else{
	  mode|=D32;
	  ret=VMEGread(sock,mode,cominfo.address,&data32,4);
	}
	if (ret<0){
	  printf("VME read failed\n"); goto error;
	}
	if (cominfo.dlength==1)	printf("Read data: %02x\n",data8);
	if (cominfo.dlength==2)	printf("Read data: %04x\n",data16);
	if (cominfo.dlength==4)	printf("Read data: %08x\n",data32);
      }else{
	if (cominfo.dlength==1) data8=(unsigned char)(cominfo.data&0xff);
	if (cominfo.dlength==2) data8=(unsigned short)(cominfo.data&0xffff);
	if (cominfo.dlength==4) data8=(unsigned int)(cominfo.data&0xffffffff);
	if (cominfo.dlength==1){
	  mode|=D8;
	  ret=VMEGwrite(sock,mode,cominfo.address,&data8,1);
	}else if (cominfo.dlength==2){
	  mode|=D16;
	  ret=VMEGwrite(sock,mode,cominfo.address,&data16,2);
	}else{
	  mode|=D32;
	  ret=VMEGwrite(sock,mode,cominfo.address,&data32,4);
	}
	if (ret<0){
	  printf("VME write failed\n"); goto error;
	}
      }
    }
    fgets(line,100,stdin);
    line[strlen(line)-1]=' ';
  }
 error:
  VMEGclose(sock);
  exit(0);
}

int anal_command(char *line, struct command_info *cominfo){
  char *token0,*token1,*token2,*token3,*token4;
  if ((token0=strtok(line," "))==NULL) goto error; /*rw*/
  if ((token1=strtok(NULL," "))==NULL) goto error; /*address mode*/
  if ((token2=strtok(NULL," "))==NULL) goto error; /*data mode*/
  if ((token3=strtok(NULL," "))==NULL) goto error; /*address*/
  if (token0[0]=='w'){
    if ((token4=strtok(NULL," "))==NULL) goto error; /*write data*/
    printf("%s:%s:%s:%s:%s\n",token0,token1,token2,token3,token4);
    cominfo->rw=1;
    sscanf(token4,"%x",&(cominfo->data));
  }else if (token0[0]=='r'){
    printf("%s:%s:%s:%s\n",token0,token1,token2,token3);
    cominfo->rw=0;
  }else{
    goto error;
  }
  cominfo->alength=0; cominfo->dlength=0;
  if (strcmp(token1,"a16")==0) cominfo->alength=2;
  if (strcmp(token1,"a24")==0) cominfo->alength=3;
  if (strcmp(token1,"a32")==0) cominfo->alength=4;
  if (strcmp(token2,"d8")==0)  cominfo->dlength=1;
  if (strcmp(token2,"d16")==0) cominfo->dlength=2;
  if (strcmp(token2,"d32")==0) cominfo->dlength=4;
  if (cominfo->alength==0||cominfo->dlength==0) goto error;
  sscanf(token3,"%x",&(cominfo->address));
  return 0;
 error:
  printf("illegal input\n");
  return -1;
}

