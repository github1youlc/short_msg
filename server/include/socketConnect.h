/*
 * socketConnect.h
 *
 *  Created on: Mar 21, 2014
 *      Author: yang
 */

#ifndef SOCKETCONNECT_H_
#define SOCKETCONNECT_H_

/* 创建一个服务器套接字
 port为端口号，backlog为服务器套接字准备accept的连接的数
 返回创建的套接字*/
int createServerSocket(int port, int backlog);

/* 从服务器套接字中接收一个新的客户端套接字。这是一个阻塞的操作，直到
 * 接收到一个连接，否则将等待完成。
 * */
int acceptClientSocket(int serverSocket);

/**
 * 创建一个客户端套接字，该套接字已经连接了，可以直接从中读取或发送信息。
 */
int createClientSocket(char * ip, int port);


/************************************************************************													client send command and receive response                    
************************************************************************/
int send_buff(char const *ip, int port,char const *buff,int len);
#endif /* SOCKETCONNECT_H_ */
