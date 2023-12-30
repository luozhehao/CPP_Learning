
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

// ���Э�飨http https ftp��
// �����ݵ�ʱ���ȷ���һ��˵������ ���߷������ҽ��������͵���ʲô����������ͷ��


#pragma pack(1)  //����1�ֽڶ���
struct FMsgHeader
{
	int type;           // ���͵��������� 0 ��ͨ���ݣ�1 ��ʾ�ļ�����
	int MsgLen;         //���ݳ���
	char FileName[30];  //�ļ������������ͨ���� ��ô��������Ч��
};
#pragma pack(0)  //����1�ֽڶ���


int main()
{
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	//����һ��socket 
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);

	//ָ��IP��ַ�Ͷ˿ں�
	struct sockaddr_in local;
	local.sin_family = AF_INET;
	local.sin_port = htons(9090);
	local.sin_addr.S_un.S_addr = INADDR_ANY;


	//��socket ��IP��ַ
	bind(sock, (struct sockaddr*)&local, sizeof(sockaddr_in));

	//�����׽���
	listen(sock, 10);

	while (1)
	{
		//�ȴ��ͻ����� (��������) ֻҪ�пͻ������ӽ����������
		SOCKET client_sock = accept(sock, NULL, NULL);
		printf("������һ��sock\n");
		while (1)
		{
			//��������
			char buff[1024] = { 0 };
			//recv �������� ����ֵ ������������շ��ؽ��յ������ݳ��ȣ�����Ͽ�����0
			// ���󷵻� -1

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

			//��ӡ����
			printf(buff);

		}

	}


	return 0;
}