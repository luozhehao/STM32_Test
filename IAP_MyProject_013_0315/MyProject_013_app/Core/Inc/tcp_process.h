/*
 * tcp_process.h
 *
 *  Created on: Sep 27, 2022
 *      Author: luozhehao
 */

#ifndef INC_TCP_PROCESS_H_
#define INC_TCP_PROCESS_H_


#include "cJSON.h"
#include "sys_type.h"


#define RECVBUFTOTOL 1024*5

typedef struct{
	char recvBuf[RECVBUFTOTOL];
	DWORD dwTotal;
}tcpRecvInfo;


typedef struct{
	WORD wSize;
	BYTE Revbuf[1024];
}RevInfo;

VOID packVwasCmd(char *buf);

VOID getCcdServiceCmdRsp(cJSON* root);

#endif /* INC_TCP_PROCESS_H_ */
