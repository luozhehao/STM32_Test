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


extern unsigned char connect_flag;


/********************************************************
 * 函数功能：创建TCP客户端
 * 形    参：ip_msg：IP信息数据结构指针
 * 返 回 值：0=成功
             1=TCP客户端线程创建失败
             2=TCP客户端数据列队创建失败
 ********************************************************/
unsigned int tcp_client_init(void *ip_msg);

/********************************************************
 * 函数功能：TCP客户端重连
 * 形    参：无
 * 返 回 值：0=成功
             1=失败
 ********************************************************/
unsigned int tcp_client_reconnect(void);

/********************************************************
 * 函数功能：获取TCP客户端连接状态
 * 形    参：无
 * 返 回 值：0=连接正常
             1=连接异常
 ********************************************************/
unsigned int tcp_client_connect_status_get(void);

/********************************************************
 * 函数功能：TCP客户端向网口发送数据
 * 形    参：pbuf：数据缓存地址
             length：发送数据字节数
 * 返 回 值：0=成功
             1=数据缓存地址为NULL
             2=数据长度错误
             3=客户端未启动
             4=连接异常
 ********************************************************/
unsigned int tcp_client_write(const unsigned char *pbuf, unsigned int length);

/********************************************************
 * 函数功能：获取TCP客户端数据列队句柄
 * 形    参：无
 * 返 回 值：数据列队句柄
 ********************************************************/
void *tcp_client_queue(void);


void tcp_client(void *parg);


#endif
