/*
 * tcp_process.c
 *
 *  Created on: Sep 27, 2022
 *      Author: luozhehao
 */

#include "tcp_process.h"

//sTcpClientRxMsg_t msg

tcpRecvInfo RecvInfo;
uint8_t printLevel=1;

void TCP_Handle(RevInfo info)
{
	uint16_t bg_num=0,  bd_num=0;
	printf("&&&&&& TCP_Handle begin ......... \n\n");

	//info.wSize
	//info.Revbuf

	if(RecvInfo.dwTotal +  info.wSize > RECVBUFTOTOL)
	{
		printf(" too long: RecvInfo.dwTotal =%d\r\n", RecvInfo.dwTotal);
		RecvInfo.dwTotal = 0;

		return;
	}
	else
	{
		memcpy((BYTE*)&RecvInfo.recvBuf[RecvInfo.dwTotal],(BYTE*)info.Revbuf, info.wSize);
		RecvInfo.dwTotal += info.wSize ;
	}

	/*****
	printf("&&& RecvInfo.dwTotal =%d, RecvInfo.recvBuf:\n\n", RecvInfo.dwTotal);
	for(int k=0;k<RecvInfo.dwTotal; k++)
	{
		if(RecvInfo.recvBuf[k]>0 && RecvInfo.recvBuf[k]<128)
			printf("%c",RecvInfo.recvBuf[k]);
		else
			printf("0x%x ",RecvInfo.recvBuf[k]);
	}
	printf("\n\n");
	*****/

	DWORD bgTotal;
	cJSON *monitor_json;

	DWORD dwCmdBegin = RecvInfo.recvBuf[0]<<24 | RecvInfo.recvBuf[1]<<16 | RecvInfo.recvBuf[2]<<8 | RecvInfo.recvBuf[3];
	if(dwCmdBegin != 0xBBBBBBBB)
	{
    	printf(" dwCmdBegin !=0xBBBBBBBB, dwCmdBegin = 0x%x \r\n", dwCmdBegin);
    	for(uint16_t bg=0; bg<RecvInfo.dwTotal; bg++)
    	{
    		if(RecvInfo.recvBuf[bg]==0xbb && RecvInfo.recvBuf[bg+1]==0xbb && RecvInfo.recvBuf[bg+2]==0xbb && RecvInfo.recvBuf[bg+3]==0xbb)
    		{
    			bg_num = bg;
    			printf("bg_num = %d\n\n", bg_num);

    			for(uint16_t bd=bg_num; bd < RecvInfo.dwTotal; bd++)
    			{
    	    		if(RecvInfo.recvBuf[bd]==0xdd && RecvInfo.recvBuf[bd+1]==0xdd && RecvInfo.recvBuf[bd+2]==0xdd && RecvInfo.recvBuf[bd+3]==0xdd)
    	    		{
    	    			bd_num = bd;
    	    			printf("bd_num = %d\n\n", bd_num);

    	    			break;
    	    		}
    			}
    			break;
    		}
    	}

    	if(bg_num==0 || bd_num==0)
    		return;

    	if(bd_num - bg_num > 10)
    	{
    		bgTotal =(RecvInfo.recvBuf[bg_num+7]<<24 | RecvInfo.recvBuf[bg_num+6]<<16 | RecvInfo.recvBuf[bg_num+5]<<8 | RecvInfo.recvBuf[bg_num+4])+12;
    		printf("bgTotal = %d\n\n", bgTotal);

			RecvInfo.recvBuf[bd_num] = '\0';
			monitor_json = cJSON_ParseWithLength( ( char *)&RecvInfo.recvBuf[bg_num + 8], bd_num-bg_num-8 );

    	}
    	else
    	{
			//RecvInfo.dwTotal = 0;
			//memset(RecvInfo.recvBuf,0,RECVBUFTOTOL);
			return;
    	}
	}
	else
	{
			printf("&&&&& TCP_Handle ...222222\n\n");

			DWORD exTotal = (RecvInfo.recvBuf[7]<<24 | RecvInfo.recvBuf[6]<<16 | RecvInfo.recvBuf[5]<<8 | RecvInfo.recvBuf[4]) + 12;
			printf("exTotal = %d\n\n", exTotal);

			DWORD exend = RecvInfo.recvBuf[exTotal-4]<<24 | RecvInfo.recvBuf[exTotal-3]<<16 | RecvInfo.recvBuf[exTotal-2]<<8 | RecvInfo.recvBuf[exTotal-1];
			printf("exend = 0x%x\n\n", exend);

			DWORD dwCmdend = RecvInfo.recvBuf[RecvInfo.dwTotal-4]<<24 | RecvInfo.recvBuf[RecvInfo.dwTotal-3]<<16 | RecvInfo.recvBuf[RecvInfo.dwTotal-2]<<8 | RecvInfo.recvBuf[RecvInfo.dwTotal-1];
			if(dwCmdend != 0xDDDDDDDD)
			{
				printf("&&& 111  dwCmdend !=0xDDDDDDDD, dwCmdend = 0x%x\r\n", dwCmdend);
				if(exend != 0xDDDDDDDD)
				return;

			}
			printf("&&&&& TCP_Handle ...333333\n\n");

			DWORD cmdTotal = (RecvInfo.recvBuf[7]<<24 | RecvInfo.recvBuf[6]<<16 | RecvInfo.recvBuf[5]<<8 | RecvInfo.recvBuf[4]) + 12;

			printf(" cmdTotal=%d, RecvInfo.dwTotal=%d\r\n", cmdTotal, RecvInfo.dwTotal);
			if(cmdTotal > RecvInfo.dwTotal)
			{
				printf("cmdTotal != RecvInfo.dwTotal,cmdTotal = %d, RecvInfo.dwTotal = %d\r\n", cmdTotal, RecvInfo.dwTotal);
			  //  recvInfo.dwTotal = 0;
				return;
			}
			if(cmdTotal < RecvInfo.dwTotal)
			{
				printf("cmdTotal != RecvInfo.dwTotal,cmdTotal=%d,RecvInfo.dwTotal=%d\r\n",cmdTotal,RecvInfo.dwTotal);

				if(exend != 0xDDDDDDDD)
				{
					RecvInfo.dwTotal = 0;
					return;
				}
			}
			printf("&&&&& TCP_Handle ...... 444444444\n\n");


			//printf("+++++RecvInfo.dwTotal = %d\nRecvInfo.recvBuf:\n", TempInfo.msgLen);
			for(int i=0; i<8; i++)
				printf("0x%x ", RecvInfo.recvBuf[i]);

			for(int k=8; k<exTotal-4; k++)
				printf("%c", RecvInfo.recvBuf[k]);

			for(int j=4; j>0; j--)
				printf("0x%x ",  RecvInfo.recvBuf[exTotal - j]);
			printf("\n\n");

		/***
			if(printLevel == 1)
			{
				printf("RecvInfo.recvBuf[i+8]:\n");
				for(int i=0; i<cmdTotal-12;i++)
					{printf("%c",RecvInfo.recvBuf[i+8]);}
				printf("\r\n");
			}
		***/

			//RecvInfo.recvBuf[RecvInfo.dwTotal-4] = '\0';
			RecvInfo.recvBuf[exTotal-4] = '\0';
			monitor_json = cJSON_ParseWithLength(( char *)&RecvInfo.recvBuf[8],cmdTotal-12);
			//cJSON *monitor_json = cJSON_ParseWithLength(( char *)&RecvInfo.recvBuf[8],cmdTotal-12);
	}

	if (monitor_json == NULL)
	{
		const char *error_ptr = cJSON_GetErrorPtr();
		if (error_ptr != NULL)
		{
			printf("Error before: %s\r\n", error_ptr);
		}

		RecvInfo.dwTotal = 0;
		cJSON_Delete(monitor_json);
		return;
	}
	const cJSON *errMsg = cJSON_GetObjectItemCaseSensitive(monitor_json, "errMsg");
	if(strcmp(errMsg->valuestring,"连接无效，请重新登录") == 0)
	{
		//loginCmd();

		RecvInfo.dwTotal = 0;
		cJSON_Delete(monitor_json);
		printf("重新登录\r\n");
		return;
	}
	const cJSON *cmd = NULL;
	cmd = cJSON_GetObjectItemCaseSensitive(monitor_json, "cmd");


	const cJSON *body = cJSON_GetObjectItemCaseSensitive(monitor_json, "body");
	const cJSON *res = cJSON_GetObjectItemCaseSensitive(body, "result");

	if (cJSON_IsString(cmd) && (cmd->valuestring != NULL))
	{
		//if(printLevel == 1)
			printf("Checking monitor \"%s\"\n", cmd->valuestring);
	}
	//if(printLevel == 1)
	{
	 printf("cmd = %s\r\n",cmd->valuestring);
	 printf("res = %s\r\n",res->valuestring);
	}


	if(strcmp(cmd->valuestring,"COMMAND_RESOURCE_ENUM") == 0)
	{
		const cJSON *body = cJSON_GetObjectItemCaseSensitive(monitor_json, "body");
		const cJSON *data = cJSON_GetObjectItemCaseSensitive(body, "data");
		for(int i=0; i<cJSON_GetArraySize (data); i++)
		{
			const cJSON *dataArray = cJSON_GetArrayItem(data, i);
			const cJSON *type = cJSON_GetObjectItemCaseSensitive(dataArray, "type");
			if(printLevel == 1)
			{
				printf("type = %s\r\n",type->valuestring);
			}
			if(strcmp(type->valuestring,"20") == 0)
			{
				//getWallCmdRsp(monitor_json);
			}
			else if(strcmp(type->valuestring,"4") == 0)
			{
				//getModeCmdRsp(monitor_json);
			}
		}
		//const cJSON *listArray = cJSON_GetArrayItem(data, 0);
		//const cJSON *listArray = cJSON_GetArrayItem(data, 0);
		if(printLevel == 1)
		{
			printf("@@@@@COMMAND_RESOURCE_ENUM\r\n");
		}
	}
	else if(strcmp(cmd->valuestring,"COMMAND_RESOURCE_QUERY") == 0)
	{
		const cJSON *body = cJSON_GetObjectItemCaseSensitive(monitor_json, "body");
		const cJSON *data = cJSON_GetObjectItemCaseSensitive(body, "data");
		for(int i=0; i<cJSON_GetArraySize (data); i++)
		{
			const cJSON *dataArray = cJSON_GetArrayItem(data, i);
			const cJSON *type = cJSON_GetObjectItemCaseSensitive(dataArray, "type");
			if(printLevel == 1)
			{
				printf("type = %s\r\n",type->valuestring);
			}
			if(strcmp(type->valuestring,"4103") == 0)
			{
				//getWallDevCmdRsp(monitor_json);
			}

		}
		//const cJSON *listArray = cJSON_GetArrayItem(data, 0);
		//const cJSON *listArray = cJSON_GetArrayItem(data, 0);
		if(printLevel == 1)
		{
			printf("@@@@@COMMAND_RESOURCE_QUERY\r\n");
		}
	}
	else if(strcmp(cmd->valuestring,"COMMAND_GET_DEVICE_OPTION") == 0)
	{
		//getDevAttCmdRsp(monitor_json);
		{
			if(printLevel == 1)
				printf("@@@@@COMMAND_GET_DEVICE_OPTION\r\n");
		}
	}
	else if(strcmp(cmd->valuestring,"CCDBOOST_COMMAND_QUERRYCCDSERVICE") == 0)
	{

		    getCcdServiceCmdRsp(monitor_json);
			if(printLevel == 1)
			{
				printf("@@@@@CCDBOOST_COMMAND_QUERRYCCDSERVICE\r\n");
			}
	}
	//"result":"false"
	//else if(strcmp(res->valuestring,"false") == 0)
	//{
	//	printf("@@@@@The result is false!\n");

	//}
	//gStaType == STA_LOGIN_IN
	else if(strcmp(cmd->valuestring,"COMMAND_USER_LOGIN") == 0)
	{
			//getLoginCmdRsp(monitor_json);
			if(printLevel == 1)
			{
				printf("@@@@@COMMAND_USER_LOGIN\r\n");
			}
	}
	// COMMAND_KEEP_LIVE
	else if(strcmp(cmd->valuestring,"COMMAND_KEEP_LIVE") == 0)
	{

			if(printLevel == 1)
			{
				printf("@@@@@COMMAND_KEEP_LIVE Heartbeat\r\n");
			}
	}


	printf("&&&&& TCP_Handle finish ... \n\n");
	RecvInfo.dwTotal = 0;
	memset(RecvInfo.recvBuf,0,RECVBUFTOTOL);
	cJSON_Delete(monitor_json);
}

//tcp_client_write(CCDQUERRY_TCP_cmd, sizeof(CCDQUERRY_TCP_cmd));

VOID packVwasCmd(char *buf)
{
	if(printLevel == 1){
		printf("packVwasCmd send:\r\n");
		for(int i=0; i<strlen(buf); i++)
		{
			printf("%c",*(buf+i));
		}
		printf("\r\n");
	}

	WORD cmdSize = strlen(buf)+12;
	//char *cmdBuf = (char *)malloc(cmdSize);
	char *cmdBuf = (char*)pvPortMalloc(cmdSize);
	if(cmdBuf == NULL)
	{
		if(printLevel == 1){ printf("string =NULL\r\n");}
		return;
	}
	char *p = cmdBuf;
	DWORD dwBegin =0xBBBBBBBB;
	DWORD dwEnd =0xDDDDDDDD;
	DWORD dwSize = strlen(buf);
	//DWORD dwBigIntSize = ((dwSize&0xff)<<24) | (((dwSize>>8)&0xff)<<16) |  (((dwSize>>16)&0xff)<<8) | ((dwSize>>24)&0xff);
	memcpy(p,(BYTE*)&dwBegin,sizeof(dwBegin));
	p = p + sizeof(dwBegin);
	memcpy(p,(BYTE*)&dwSize,sizeof(dwSize));
	p = p + sizeof(dwSize);
	memcpy(p,(BYTE*)buf,strlen(buf));
	p = p + strlen(buf);
	memcpy(p,(BYTE*)&dwEnd,sizeof(dwEnd));

	//sendWifiInfo(cmdBuf,cmdSize);

	//TCP_Send_To_Server((BYTE*)cmdBuf,cmdSize);   //2022.6.10 add
	//tcp_client_send_data((BYTE*)cmdBuf, cmdSize);  //2022.9.27 add
	tcp_client_write((BYTE*)cmdBuf, cmdSize);

	vPortFree(cmdBuf);
}


VOID getCcdServiceCmdRsp(cJSON* root)
{

	const cJSON *body = cJSON_GetObjectItemCaseSensitive(root, "body");
	const cJSON *port = cJSON_GetObjectItemCaseSensitive(body, "port");
	//gNetServiceCfg.port = atoi(port->valuestring);

	uint16_t server_port =  atoi(port->valuestring);
	printf("++++++++++ server_port = %d\n\r", server_port);

/*******
	//gStaType = STA_LOGINPORT;
	printf("getCcdServiceCmdRsp gNetServiceCfg.port = %d\n\r",gNetServiceCfg.port);
	//SetNetServiceCfg(gNetServiceCfg.port) ;

	if(gStaType == STA_CCDPORT_CONNECT)
	{
		g_Server_Port = gNetServiceCfg.port;
		Server_Port_flag = 1;
		restart_tcpclient(gNetServiceCfg.port, 1);

		printf("(&&&&&&&&&&&&SetNetServiceCfg gNetServiceCfg.port = %d\n\r",gNetServiceCfg.port);
		//SaveNetServiceCfg();
		gStaType = STA_LOGINPORT;
	}
******/


}






