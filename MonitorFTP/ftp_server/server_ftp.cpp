
#include <stdio.h>
#include <stdlib.h>
#include <WinSock2.h>
#pragma comment(lib, "ws2_32.lib")

/***
socket(
_In_ int af,
_In_ int type,
_In_ int protocol
);
***/

// 在windows平台使用socket必须开启网络权限


//typedef struct sockaddr_in {
//    short   sin_family;
//    USHORT sin_port;
//    IN_ADDR sin_addr;
//    CHAR sin_zero[8];
//} SOCKADDR_IN, * PSOCKADDR_IN;

//bind(
//    SOCKET s,
//    sockaddr FAR* name,
//    int namelen
//);

//listen(
//    _In_ SOCKET s,
//    _In_ int backlog
//);

//accept(
//    _In_ SOCKET s,
//     struct sockaddr FAR * addr,   表示客户端IP地址
//    _Inout_opt_ int FAR * addrlen  //后面两个参数都可以不填
//    );


//recv(
//    _In_ SOCKET s,  //已经建立连接的socket
//    char FAR* buf,  //用来接收数据的缓冲区
//    _In_ int len,   //接收多少数据
//    _In_ int flags    //接收模式：阻塞接收；非阻塞接收；只接收返回值；拷贝数据但不从缓冲区取出（是否断线判断）
//);

// 设计协议（http https ftp）
// 发数据的时候先发送一个说明数据 告诉服务器我接下来发送的是什么东西（数据头）


#pragma pack(1)  //按照1字节对齐
struct FMsgHeader
{
	int type;           // 发送的数据类型 0 普通数据，1 表示文件数据
	int MsgLen;         //数据长度
	char FileName[30];  //文件名（如果是普通数据 那么本属性无效）
};
#pragma pack(0)  //按照1字节对齐


int main()
{
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	//创建一个socket 
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);

	//指定IP地址和端口号
	struct sockaddr_in local;
	local.sin_family = AF_INET;
	local.sin_port = htons(9090);
	local.sin_addr.S_un.S_addr = INADDR_ANY;


	//绑定socket 和IP地址
	bind(sock, (struct sockaddr*)&local, sizeof(sockaddr_in));

	//监听套接字
	listen(sock, 10);

	while (1)
	{
		//等待客户连接 (阻塞函数) 只要有客户端连接进来解除阻塞
		SOCKET client_sock = accept(sock, NULL, NULL);
		printf("监听到一个sock\n");
		while (1)
		{
			//接收数据
			char buff[1024] = { 0 };
			//recv 阻塞函数 返回值 ，如果正常接收返回接收到的数据长度；如果断开返回0
			// 错误返回 -1

			struct FMsgHeader msgHeader;

			int ret = recv(client_sock, (char*)&msgHeader, sizeof(struct FMsgHeader), 0);
			if (ret < +0)
				break;
			if (msgHeader.type == 0)
			{
			}
			else if (msgHeader.type == 1)
			{
				int filesize = msgHeader.MsgLen;
				FILE* fp = fopen(msgHeader.FileName, "wb+");
				char buff[1024] = { 0 };
				while (filesize>0)
				{
					int ret = recv(client_sock, buff, 1024 < filesize ? 1024 : filesize, 0);
					filesize -= ret;
					fwrite(buff, 1, ret, fp);
				}
				fclose(fp);
			}


			printf("ret = %d\n", ret);

			//打印数据
			printf(buff);

		}

	}


	return 0;
}