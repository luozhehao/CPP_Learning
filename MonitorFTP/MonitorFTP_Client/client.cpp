/***
1.�����������У��ͻ��˺����У�
2.�ͻ����������ӵ���������ɨ�貢��ȡ�ͻ�������ָ��Ŀ¼������Ŀ¼�ĵ� .jpg�ļ������ҷ��͸���������
3.���������տͻ��˷��͵��ļ����ݣ����ұ�������
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

// ��windowsƽ̨ʹ��socket���뿪������Ȩ��

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
//     struct sockaddr FAR * addr,   ��ʾ�ͻ���IP��ַ
//    _Inout_opt_ int FAR * addrlen  //�����������������Բ���
//    );


//recv(
//    _In_ SOCKET s,  //�Ѿ��������ӵ�socket
//    char FAR* buf,  //�����������ݵĻ�����
//    _In_ int len,   //���ն�������
//    _In_ int flags    //����ģʽ���������գ����������գ�ֻ���շ���ֵ���������ݵ����ӻ�����ȡ�����Ƿ�����жϣ�
//);

//send(
//    _In_ SOCKET s,
//    const char FAR* buf,
//    _In_ int len,
//    _In_ int flags
//);

void findsubdir(string path, string mode, set<string>& files);

#pragma pack(1)  //����1�ֽڶ���
struct FMsgHeader
{
	int type;           // ���͵��������� 0 ��ͨ���ݣ�1 ��ʾ�ļ�����
	int MsgLen;         //���ݳ���
	char FileName[30];  //�ļ������������ͨ���� ��ô��������Ч��
};

typedef struct FileMsgHeader
{
	int type;           // ���͵��������� 0 ��ͨ���ݣ�1 ��ʾ�ļ�����
	int MsgLen;         //���ݳ���
	char FileName[128];  //�ļ������������ͨ���� ��ô��������Ч��
}msgHeaderDef;

#pragma pack(0)  //����1�ֽڶ���

int jpg_cnt = 0;


char* mp4_file = "123.mp4";

int main()
{
	//��������Ȩ��
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);


	//ɨ��ָ���ļ��У������������
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
		//����ȡ���������ӷŵ�������,�Ž�ȥ֮ǰ����Ҫ�жϣ��Ƿ���ʹ�

		printf("%s\n", (*it).c_str());

	}
	//strrchr();//

	//����һ��socket 
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	printf("socket finish\n");

	//ָ��IP��ַ�Ͷ˿ں�
	struct sockaddr_in serverAddr;
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(9090);
	// ֱ����IP��ַ/ ��ػ���ַ inet_addr("127.0.0.1");
	// �ӵ��ʮ����ת��Ϊ���� 
	//INADDR_LOOPBACK
	serverAddr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");


	//���ӷ�����
	int ret = connect(sock, (struct sockaddr*)&serverAddr, sizeof(sockaddr));
	if (ret == -1)
	{
		printf("���ӷ�����ʧ��\n");
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
			//����ȡ���������ӷŵ�������,�Ž�ȥ֮ǰ����Ҫ�жϣ��Ƿ���ʹ�

			printf("%s\n", (*it).c_str());
			
			//��ȡ�ļ�����
			FILE* fp = fopen( (*it).c_str(), "rb+");
			//FILE* fp = fopen(mp4_file , "rb+");
			fseek(fp, 0, SEEK_END);
			int filesize = ftell(fp);
			fclose(fp);

			printf("filesize = %d\n", filesize);

			msgHeaderDef msgHeader;
			memset(&msgHeader, 0, sizeof(msgHeaderDef));
			msgHeader.type = 1;
			//�ļ�����
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
		//��ȡ�ļ�����
		FILE* fp = fopen("123.mp4", "rb+");
		fseek(fp, 0, SEEK_END);
		int filesize = ftell(fp);
		fclose(fp);

		struct FMsgHeader msgHeader;
		memset(&msgHeader, 0, sizeof(struct FMsgHeader));
		msgHeader.type = 1;
		//�ļ�����
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
	//�ȴ��ͻ����� (��������) ֻҪ�пͻ������ӽ����������
	SOCKET client_sock = accept(sock, NULL, NULL);

	//��������
	char buff[1024] = { 0 };
	//recv �������� ����ֵ ������������շ��ؽ��յ������ݳ��ȣ�����Ͽ�����0
	// ���󷵻� -1

	int ret = recv(client_sock, buff, 1024, 0);
	if (ret < +0)
	break;

	printf("len = %d\n", ret);
	//��ӡ����
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
			//�ж��Ƿ�����Ŀ¼  
			printf("file.attrib = %d, file.name = %s \n\n", file.attrib, file.name);
			if (file.attrib & _A_SUBDIR)
			{
				//�ж��Ƿ�Ϊ"."��ǰĿ¼��".."��һ��Ŀ¼
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
			//�ж��Ƿ�����Ŀ¼  
			printf("file_sub.attrib = %d, file_sub.name = %s \n", file_sub.attrib, file_sub.name);
			if (file_sub.attrib & _A_SUBDIR)
			{
				//�ж��Ƿ�Ϊ"."��ǰĿ¼��".."��һ��Ŀ¼
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