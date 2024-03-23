/*
 * tcpclient.c
 *
 *  Created on: 2022年9月9日
 *      Author: luozhehao
 */


//tcp_clent.c
#include "FreeRTOS.h"
#include "queue.h"
#include "task.h"
//#include "sys_eth.h"
#include "lwip/api.h"
#include "tcp_client.h"

#include "lwip.h"
#include "netif.h"
#include "lwipopts.h"
#include "lwip/api.h"

#include "FreeRTOSConfig.h"

#include "tcp_process.h"

/****/

 typedef struct
 {
	 uint8_t localip[4];     //本机IP地址
	 uint8_t remoteip[4];	 //远端主机IP地址
	 uint16_t localport;
	 uint16_t remoteport;

 }sLwipDev_t;


/***/


#include "main.h"
#include "tcp_client.h"
#include "lwip/opt.h"
#include "lwip/tcpip.h"
#include "lwip/sys.h"
#include "lwip/api.h"
#include "lwip/sockets.h"

#include "FreeRTOS.h"
#include "semphr.h"
#include "task.h"
#include "string.h"

#define TCP_CLIENT_TASK_PRIO	( tskIDLE_PRIORITY + 4 )
#define TCP_CLIENT_STACK_SIZE	( configMINIMAL_STACK_SIZE * 20 )


#define SERVER_PORT  12345

uint8_t server_addr[] = "192.168.1.105";

static sys_thread_t lwip_thread;


//客户端只需要一个套接字文件描述符，用于和服务器通信
int serverSocket = -1;

void tcp_client_disconnect(void)
{
	if(serverSocket != -1){
		close(serverSocket);
		serverSocket = -1;

		vTaskDelete(lwip_thread);
		lwip_thread = NULL;
	}
}

int8_t tcp_client_send_data(uint8_t *pData, uint16_t len)
{
	int8_t ret = -1;

	if(serverSocket == -1){
		return ret;
	}

	ret = send(serverSocket, pData, len, 0); //向服务端发送消息
	if (ret < 0) {
		printf("[TCP Client] Send data is error!!!\r\n");
	}

	return ret;
}

static void tcp_client_socket_thread(void *arg)
{
	//描述服务器的socket
	struct sockaddr_in serverAddr;

	char recvbuf[1000]; //存储 接收到的信息
	int iDataNum;

	if((serverSocket = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP)) < 0)
	{
		printf("[TCP Client] Create socket is fail!!!\r\n");
		goto EXIT;
	}

	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(SERVER_PORT);

	//指定服务器端的ip，地址：192.168.1.100
	//inet_addr()函数，将点分十进制IP转换成网络字节序IP
	serverAddr.sin_addr.s_addr = inet_addr(server_addr);

	if(connect(serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0)
	{
		printf("[TCP Client] Connect is fail!!!\r\n");
		goto EXIT;
	}

	printf("[TCP Client] Connect server is successful!!!\r\n");

	while(1)
	{
		iDataNum = recv(serverSocket, recvbuf, sizeof(recvbuf), 0); //接收服务端发来的消息
		recvbuf[iDataNum] = '\0';
		printf("[Recv Data]%s\r\n", recvbuf);
	}

EXIT:
	vTaskDelete(lwip_thread);
	lwip_thread = NULL;
	return;
}


void tcp_client_init(void)
{
	lwip_thread = sys_thread_new("tcp_client_socket", tcp_client_socket_thread, NULL, TCP_CLIENT_STACK_SIZE, TCP_CLIENT_TASK_PRIO);
}





/***********

#define RECV_BUF_SIZE			4096
static uint8_t recvBuf[RECV_BUF_SIZE];

static struct netconn *tcp_client_handle = NULL;

void tcp_client_disconnect(void)
{
	struct netconn *conn = tcp_client_handle;

	if(conn != NULL){
		netconn_close(conn);
		netconn_delete(conn);

		vTaskDelete(lwip_thread);   //lwip_thread->thread_handle
		lwip_thread = NULL;
	}
}

int8_t tcp_client_send_data(uint8_t *pData, uint16_t len)
{
	int8_t ret = -1;
	struct netconn *conn = tcp_client_handle;

	if(conn == NULL){
		return ret;
	}

	ret = netconn_write(conn, pData, len, NETCONN_COPY);
	if (ret != ERR_OK) {
		printf("tcpecho: netconn_write: error \"%s\"\n", lwip_strerr(ret));
	}

	return ret;
}

#define LOCAL_PORT  5678
#define  DEST_IP_ADDR0  192
#define  DEST_IP_ADDR1  168
#define  DEST_IP_ADDR2 1
#define  DEST_IP_ADDR3 105
#define  DEST_PORT    12345

#define BUFMAXSIZE 1024

typedef struct{
	char msgBuf[BUFMAXSIZE];
	DWORD msgLen;
}tcpTempInfo;


CCMRAM tcpTempInfo tcpRecvbuf;
CCMRAM int ccmtest[1000];

u32_t data_len = 0;
struct pbuf *q;

static void tcp_client_netconn_thread(void *arg)
{
	struct netconn *conn;
	struct netbuf *buf;
	ip_addr_t DestIPaddr;
	err_t err;
	LWIP_UNUSED_ARG(arg);


	//for(int i=0; i<1000;i++)
	//{
		//ccmtest[i]=i;
	//}
	//printf("ccmtest[105] = %d \n\n",ccmtest[105]);

	/* Bind to TCP Client port with default IP address */

/************
conn = netconn_new(NETCONN_TCP);
	if(conn == NULL){
		printf("[TCP Client]netconn_new: invalid conn\r\n");
		goto EXIT;
	}

	netconn_bind(conn, IP4_ADDR_ANY, NULL);

	IP4_ADDR( &DestIPaddr, DEST_IP_ADDR0, DEST_IP_ADDR1, DEST_IP_ADDR2, DEST_IP_ADDR3 );
	err = netconn_connect(conn, &DestIPaddr, DEST_PORT);
	if(err != ERR_OK){
		printf("[TCP Client]netconn_connect is fail!!!\r\n");
		goto EXIT;
	}

	tcp_client_handle = conn;
	printf("[TCP Client] Connect server is successful!!!\r\n");

	do {
		err = netconn_recv(conn, &buf);

		if (err == ERR_OK) {
/****************************
			uint8_t len = (buf->p->len < RECV_BUF_SIZE) ? buf->p->len : RECV_BUF_SIZE;

			printf("[Recv Data] len = %d\r\n", len);

			MEMCPY(recvBuf, buf->p->payload, len);
			recvBuf[len] = '\0';
			printf("[Recv Data]%s\r\n", (char *)recvBuf);
***********************/
/*******
			uint32_t length = buf->p->tot_len;

			printf("[Recv Data]length total = %d\r\n", length);

			printf("[Recv Data]tcpRecvbuf.msgLen = %d\r\n", tcpRecvbuf.msgLen);

			for(q=buf->p;q!=NULL;q=q->next)
			{
				printf("[Recv Data] q->len = %d\r\n", q->len);
				if(q->len > (BUFMAXSIZE - data_len))
				{
					memcpy(tcpRecvbuf.msgBuf + data_len, q->payload, (BUFMAXSIZE - data_len));
				}
				else
				{
					memcpy(tcpRecvbuf.msgBuf + data_len,q->payload,q->len);
				}

				data_len += q->len;

				if(data_len > BUFMAXSIZE) break;
			}

			tcpRecvbuf.msgLen = data_len;
			tcpRecvbuf.msgBuf[data_len] = '\0';
			data_len = 0;

			printf("[Recv Data] %s\r\n", tcpRecvbuf.msgBuf);
		}

		if (buf != NULL) {
			netbuf_delete(buf);
		}
	} while (1);

EXIT:
	vTaskDelete(lwip_thread);
	lwip_thread = NULL;   //lwip_thread->thread_handle
	return;
}

void tcp_client_init(void)
{
	lwip_thread = sys_thread_new("tcp_client_netconn", tcp_client_netconn_thread, NULL, TCP_CLIENT_STACK_SIZE, TCP_CLIENT_TASK_PRIO);
}

***********/



