
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

//send(
//    _In_ SOCKET s,
//    const char FAR* buf,
//    _In_ int len,
//    _In_ int flags
//);

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
	//开启网络权限
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	//创建一个socket 
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	printf("socket finish\n");

	//指定IP地址和端口号
	struct sockaddr_in serverAddr;
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(9090);
	// 直接填IP地址/ 填回环地址 inet_addr("127.0.0.1");
	// 从点分十进制转换为整数 
	//INADDR_LOOPBACK
	serverAddr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");


	//连接服务器
	int ret = connect(sock, (struct sockaddr*)&serverAddr, sizeof(sockaddr));
	if (ret == -1)
	{
		printf("连接服务器失败\n");
		exit(-1);
	}

	//获取文件长度
	FILE* fp = fopen("123.mp4", "rb+");
	fseek(fp, 0, SEEK_END);
	int filesize = ftell(fp);
	fclose(fp);

	struct FMsgHeader msgHeader;
	memset(&msgHeader, 0, sizeof(struct FMsgHeader));
	msgHeader.type = 1;
	//文件长度
	msgHeader.MsgLen = filesize;
	memcpy(msgHeader.FileName, "123.mp4", strlen("123.mp4"));

	send(sock, (char*)&msgHeader, sizeof(struct FMsgHeader), 0);

	char* filecontent = (char*)malloc(filesize);
	fp = fopen("123.mp4", "rb+");
	fread(filecontent, 1, filesize, fp);
	fclose(fp);
	send(sock, (char*)filecontent, filesize, 0);


	system("pause");

	/***
	while (1)
	{
	//等待客户连接 (阻塞函数) 只要有客户端连接进来解除阻塞
	SOCKET client_sock = accept(sock, NULL, NULL);

	//接收数据
	char buff[1024] = { 0 };
	//recv 阻塞函数 返回值 ，如果正常接收返回接收到的数据长度；如果断开返回0
	// 错误返回 -1

	int ret = recv(client_sock, buff, 1024, 0);
	if (ret < +0)
	break;

	printf("len = %d\n", ret);
	//打印数据
	printf(buff);

	}
	***/




	return 0;
}