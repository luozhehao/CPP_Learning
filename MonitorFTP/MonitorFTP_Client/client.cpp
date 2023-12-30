/***
1.服务器先运行，客户端后运行；
2.客户端上线连接到服务器后，扫描并读取客户端主机指定目录及其子目录的的 .jpg文件，并且发送给服务器；
3.服务器接收客户端发送的文件数据，并且保存下来
***/


#include <stdio.h>
#include <stdlib.h>
#include <WinSock2.h>
#include <iostream>
#include <string>
#include <map>
#include <set>
#include <io.h>

#pragma comment(lib, "ws2_32.lib")

using namespace std;

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

void findsubdir(string path, string mode, set<string>& files);

#pragma pack(1)  //按照1字节对齐
struct FMsgHeader
{
	int type;           // 发送的数据类型 0 普通数据，1 表示文件数据
	int MsgLen;         //数据长度
	char FileName[30];  //文件名（如果是普通数据 那么本属性无效）
};

typedef struct FileMsgHeader
{
	int type;           // 发送的数据类型 0 普通数据，1 表示文件数据
	int MsgLen;         //数据长度
	char FileName[128];  //文件名（如果是普通数据 那么本属性无效）
}msgHeaderDef;

#pragma pack(0)  //按照1字节对齐

int jpg_cnt = 0;


char* mp4_file = "123.mp4";

int main()
{
	//开启网络权限
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);


	//扫描指定文件夹，并且逐个发送
	printf("\n\n findsubdir ... 001\n\n" );
	set<string> files;
	string path = "O:\\Picture";
	string mode = "\\*.jpg";
	printf("\n\n findsubdir ... 002\n\n");
	findsubdir(path, mode, files);

	int emt_files = files.empty();
	int size_files = files.size();
	printf("\n\n files, ret_url = %d, size_url = %d\n\n", emt_files, size_files);

	printf("find out files: \n");
	for (set<string>::iterator it = files.begin(); it != files.end(); it++)
	{
		//printf("%s\n", (*it).c_str());
		//将提取出来的链接放到队列中,放进去之前，需要判断，是否访问过

		printf("%s\n", (*it).c_str());

	}
	//strrchr();//

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
		system("pause");
		exit(-1);
	}
	printf("connect success ...\n");

	set<string>::iterator it = files.begin();

	while (1)
	{
		for (; it != files.end(); it++)
		{
			//printf("%s\n", (*it).c_str());
			//将提取出来的链接放到队列中,放进去之前，需要判断，是否访问过

			printf("%s\n", (*it).c_str());
			
			//获取文件长度
			FILE* fp = fopen( (*it).c_str(), "rb+");
			//FILE* fp = fopen(mp4_file , "rb+");
			fseek(fp, 0, SEEK_END);
			int filesize = ftell(fp);
			fclose(fp);

			printf("filesize = %d\n", filesize);

			msgHeaderDef msgHeader;
			memset(&msgHeader, 0, sizeof(msgHeaderDef));
			msgHeader.type = 1;
			//文件长度
			msgHeader.MsgLen = filesize;
			//memcpy(msgHeader.FileName, mp4_file, strlen((*it).c_str()));
			memcpy(msgHeader.FileName, (*it).c_str(), strlen((*it).c_str()));
			printf("msgHeader.FileName = %s\n", msgHeader.FileName);

			send(sock, (char*)&msgHeader, sizeof(msgHeaderDef), 0);

			char* filecontent = (char*)malloc(filesize);
			fp = fopen( (*it).c_str() , "rb+");
			//fp = fopen(mp4_file, "rb+");
			fread(filecontent, 1, filesize, fp);
			fclose(fp);
			send(sock, (char*)filecontent, filesize, 0);

		}

		/***
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
		***/
	}

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



void findsubdir(string path, string mode, set<string>& files)
{
	_finddata_t file;
	intptr_t HANDLE;
	string Onepath = path + mode;
	HANDLE = _findfirst(Onepath.c_str(), &file);
	if (HANDLE == -1L)
	{
		cout << "can not match the folder path" << endl;
		//system("pause");
	}
	else
	{
		do {
			//判断是否有子目录  
			printf("file.attrib = %d, file.name = %s \n\n", file.attrib, file.name);
			if (file.attrib & _A_SUBDIR)
			{
				//判断是否为"."当前目录，".."上一层目录
				if ((strcmp(file.name, ".") != 0) && (strcmp(file.name, "..") != 0))
				{
					string newPath = path + "\\" + file.name;
					findsubdir(newPath, mode, files);
				}
			}
			else
			{
				cout << "jpg: " << path << "\\" << file.name << " " << endl;
				string jpg_str = path + "\\" + file.name;
				files.insert(jpg_str);
				printf("\n\n files, files.size() = %d\n\n", files.size());
				cout << "files.size()=" << files.size() << endl;
				cout << jpg_str << endl;

				//cout << path << "\\" << file.name << " " << endl;
				jpg_cnt++;
				printf("jpg_cnt = %d\n\n", jpg_cnt);
			}
		} while (_findnext(HANDLE, &file) == 0);
		_findclose(HANDLE);
	}

	// subdir
	_finddata_t file_sub;
	intptr_t HANDLE_SUB;
	string Onepath_sub = path + "\\*";
	HANDLE_SUB = _findfirst(Onepath_sub.c_str(), &file_sub);
	if (HANDLE_SUB == -1L)
	{
		cout << "SUB can not match the folder path" << endl;
		//system("pause");
	}
	else
	{
		do {
			//判断是否有子目录  
			printf("file_sub.attrib = %d, file_sub.name = %s \n", file_sub.attrib, file_sub.name);
			if (file_sub.attrib & _A_SUBDIR)
			{
				//判断是否为"."当前目录，".."上一层目录
				if ((strcmp(file_sub.name, ".") != 0) && (strcmp(file_sub.name, "..") != 0))
				{

					string newPath = path + "\\" + file_sub.name;
					cout << "newPath: " << newPath << endl;
					findsubdir(newPath, mode, files);
				}
			}
			else
			{
				cout << " SUB path:" << path << "\\"<<file_sub.name << " " << endl;
				//cout << path << file_sub.name << " " << endl;
				//jpg_cnt++;
				//printf("sub jpg_cnt = %d\n\n", jpg_cnt);
			}
		} while (_findnext(HANDLE_SUB, &file_sub) == 0);
		_findclose(HANDLE_SUB);
	}
}