
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

//send(
//    _In_ SOCKET s,
//    const char FAR* buf,
//    _In_ int len,
//    _In_ int flags
//);

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
	//��������Ȩ��
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);

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
		exit(-1);
	}

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