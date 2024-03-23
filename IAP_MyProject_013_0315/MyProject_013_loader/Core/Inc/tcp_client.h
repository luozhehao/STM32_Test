/*
 * tcpclient.h
 *
 *  Created on: 2022年9月9日
 *      Author: luozhehao
 */

#ifndef INC_TCPCLIENT_H_
#define INC_TCPCLIENT_H_



#endif /* INC_TCPCLIENT_H_ */



/***
 *
 *   http://t.zoukankan.com/icode-wzc-p-12929136.html
 *
 * ***/



//tcp_client.h

#ifndef __TCP_CLIENT_H
#define __TCP_CLIENT_H


#define CCMRAM __attribute__((section(".ccmram")))

//状态机
typedef enum
{
	STA_TCP_BEGIN = 0,
	STA_TCP_NO_CONNECT = 1,//网络未连接
	STA_TCP_CONNECT = 2,//网络连接，PORT CCD服务未连接
	STA_CCDPORT = 3,//CCD服务连接
	STA_CCDPORT_CONNECT = 4,//CCD服务连接
	STA_LOGINPORT = 5,//CCD服务连接
	STA_LOGINPORT_CONNECT = 6,//CCD服务连接
	STA_LOGIN_IN = 7,//已登录
	STA_LOGIN_OUT = 8//已登出
}StaType;


int8_t tcp_client_send_data(uint8_t *pData, uint16_t len);

void tcp_client_init(void);


#endif
