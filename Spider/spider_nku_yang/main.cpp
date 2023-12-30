


#include "main.h"



int main(int argc, char** argv[])
{
	InitNetwork();
	//URL��ַ���Ը����Լ�ϲ��������ģ�����ֻ��һ��ʾ��
	//string beginURL = "http://www.win4000.com/meitu.html";
	string beginURL = "http://www.mmonly.cc/mmtp/nymn/306559.html";

	SnapImageFromUrl(beginURL);
	return 0;
}



void InitNetwork()
{
	WORD wVersionRequested;
	WSADATA wsaData;
	int err;
	wVersionRequested = MAKEWORD(2, 2);//�涨ʹ�õ�Э��汾

	err = WSAStartup(wVersionRequested, &wsaData);
	if (err != 0)
	{
		return;
	}
}

void SnapImageFromUrl(string url)
{
	//���ù�ȱ����ķ�ʽ������վ�����е�ͼƬ
	queue<string> queUrls;
	queUrls.push(url);

	map<string, int> tempMap;   //���ڼ�¼�ѷ��ʹ�����ַ

	while (!queUrls.empty())
	{
		try {
			string curUrl = queUrls.front();   //ȡ���������ʼ���Ž�ȥ��Ԫ��
			queUrls.pop();

			//�����о�
			if (curUrl.size() > 256) continue;

			//���Ѿ����ʹ��ķŵ�map��
			tempMap[curUrl]++;

			printf("��ǰ����ģ�%d %s\n", queUrls.size(), curUrl.c_str());

			//��curUrl�µ�ͼƬȫ��������
			string hostAddr, fileAddr;
			SOCKET socket = 0;
			//��ȡ���������ļ���
			GetHostAddrByUrl(curUrl, hostAddr, fileAddr);

			if ((socket = ConnectAddr(hostAddr)) == 0)
			{
				printf("��������ʧ�� : %s\n", hostAddr);
			}

			cout << "hostAddr: " << hostAddr << endl;
			cout << "fileAddr: " << fileAddr << endl;


			//��ȡ��ҳԴ����
			string html = GetHtmlCode(socket, hostAddr, fileAddr);
			if (!html.empty())
			{
				cout << "html:" << endl;
				cout <<  html << endl;

				//��ȡ����ҳ�е�ͼƬ����
				set<string> imageUrl;
				GetImagesUrlfromHtml(html, imageUrl);

				int ret = imageUrl.empty();
				int size = imageUrl.size();
				printf("\n\nimageUrl, ret = %d, size = %d\n\n", ret, size);

				for (set<string>::iterator it = imageUrl.begin(); it != imageUrl.end(); it++)
				{
					printf("for iterator it:\n");
					printf("%s\n", (*it).c_str());
					SaveImage(*it);   //ȥŪ��·��
				}

				printf("SnapImageFromUrl ...002, html:\n");
				//��ȡ��ǰ��ҳ�е���������,���ҽ���ŵ�������
				set<string> urls;
				GetUrlfromHtml(html, urls);

				int ret_url = urls.empty();
				int size_url = urls.size();
				printf("\n\n urls, ret_url = %d, size_url = %d\n\n", ret_url, size_url);

				for (set<string>::iterator it = urls.begin(); it != urls.end(); it++)
				{
					//printf("%s\n", (*it).c_str());
					//����ȡ���������ӷŵ�������,�Ž�ȥ֮ǰ����Ҫ�жϣ��Ƿ���ʹ�
					if (tempMap[(*it)] == 0)  //����ַû�б����ʹ�
					{
						queUrls.push((*it));
						printf("queUrls it:\n");
						printf("%s\n", (*it).c_str());
						
					}
					else
					{
						printf("%s �ѷ��ʹ�\n", (*it).c_str());
						printf("SnapImageFromUrl ...004\n");
						
					}
				}


			}
		}
		catch (...) {
			int err = 11;
		}
	}
	int a = 20;
}



string GetHtmlCode(unsigned int socket, string hostAddr, string fileAddr)
{
	//ģ��httpЭ��
	string reqInfo;
	reqInfo = reqInfo + "GET " + fileAddr + " HTTP/1.1\r\n";
	reqInfo = reqInfo + "Host: " + hostAddr + "\r\n";
	reqInfo = reqInfo + "Connection: Close\r\n";
	reqInfo = reqInfo + "User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/119.0.0.0 Safari/537.36 OPR/105.0.0.0\r\n";
	reqInfo = reqInfo + "\r\n";

	cout << "reqInfo:\n"<< reqInfo << endl;

	//��ģ���httpЭ�鷢�͸�����
	int sendLen = send(socket, reqInfo.c_str(), reqInfo.size(), 0);
	if (sendLen <= 0)
		return "";

	//�������httpЭ��֮�󣬸��ҷ�����ҳ��Դ����,������
	string htmlCode;
	while (1)
	{
		char recvBuff[1024] = { 0 };
		int recvLen = recv(socket, recvBuff, 1024, 0);
		if (recvLen <= 0)
			break;
		else
		{
			htmlCode += recvBuff;   //����ҳԴ�����ۼ�
		}
	}
	//���ݵ�����excel(com)

	//printf("%s\n", htmlCode.c_str());
	//....

	closesocket(socket);

	return htmlCode;
}



unsigned int ConnectAddr(string hostAddr)
{
	//�����׽���
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if (INVALID_SOCKET == sock)
		return INVALID_SOCKET;

	//��������ַ��ȡ��Ӧ��IP��ַ
	hostent* pHost = gethostbyname(hostAddr.c_str());
	if (pHost == 0)
		return 0;

	//׼��Ҫ���ӵ�IP��ַ
	sockaddr_in addr;
	addr.sin_family = AF_INET;   //��ַ����
	addr.sin_port = htons(80);   //�˿ں�, h(host) to n(net) s(short), �������ֽ���ת���������ֽ���
	memcpy(&(addr.sin_addr.S_un.S_addr), pHost->h_addr, 4);   //ip��ַ����

															  //���ӷ�����
	if (SOCKET_ERROR == connect(sock, (sockaddr*)&addr, sizeof(sockaddr)))
		return 0;

	return sock;
}


void GetHostAddrByUrl(string url, string& hostAddr, string& fileAddr)
{
	//strstr();   //�����ַ�������
	//strtok();   //�ָ��ַ�������
	char szArr[256] = { 0 }, szHostAddr[256] = { 0 }, szfileAddr[256] = { 0 };
	const char* pFind = strstr(url.c_str(), "http://");
	if (pFind)
	{
		strcpy(szArr, url.c_str() + strlen("http://"));
	}
	else
	{
		strcpy(szArr, url.c_str());
	}

	char* pFileAddr = strstr(szArr, "/");
	if (pFileAddr)
	{
		strncpy(szHostAddr, szArr, pFileAddr - szArr);
		strcpy(szfileAddr, pFileAddr);
		//memcpy();
	}

	hostAddr = szHostAddr;
	fileAddr = szfileAddr;
}


void GetUrlfromHtml(const string& html, set<string>& Url)
{
	smatch mat;    //����ƥ���

	regex patten("a href=\"https://(www..*?\\.html)\"");

	//��һ����Χ�ڽ������������������Ľ����ƥ�䵽smatch��
	string::const_iterator beginIter = html.begin();
	string::const_iterator endIter = html.end();
	while (regex_search(beginIter, endIter, mat, patten))
	{
		//�ҵ���ƥ����, ������mat��
		string url(mat[1].first, mat[1].second);

		Url.insert(url);

		beginIter = mat[0].second;   //�޸���������ʼλ��
	}
}



//����������ʽ����ȡͼƬ����
//����ƥ��
//��������
//�����޸�
void GetImagesUrlfromHtml(const string& html, set<string>& imageUrl)
{
	smatch mat;    //����ƥ���

	//regex patten("\"http://(.*?\\.jpg)\"");
	regex patten("\"https://(.*?\\.jpg)\"");

	//��һ����Χ�ڽ������������������Ľ����ƥ�䵽smatch��
	string::const_iterator beginIter = html.begin();
	string::const_iterator endIter = html.end();
	while (regex_search(beginIter, endIter, mat, patten))
	{
		//�ҵ���ƥ����, ������mat��
		string url(mat[1].first, mat[1].second);

		imageUrl.insert(url);

		beginIter = mat[0].second;   //�޸���������ʼλ��
	}
}


void SaveImage(const string& imageUrl)
{
	//1.��ȡ���������ļ���
	string hostAddr, fileAddr;
	printf("SaveImage ...001\n\n");
	GetHostAddrByUrl(imageUrl, hostAddr, fileAddr);

	printf("SaveImage ...002\n\n");

	unsigned int sock = ConnectAddr(hostAddr);
	if (sock > 0)
	{
		printf("SaveImage ...003\n\n");
		_SaveJPG(sock, hostAddr, fileAddr);
	}
}


void _SaveJPG(unsigned int socket, string hostAddr, string fileAddr)
{
	//ģ��httpЭ��
	string reqInfo = "GET " + fileAddr + " HTTP/1.1\r\n" +
		"HOST:" + hostAddr + "\r\n" +
		"Connection:Close\r\n\r\n";

	//��ģ���httpЭ�鷢�͸�����
	int sendLen = send(socket, reqInfo.c_str(), reqInfo.size(), 0);
	if (sendLen <= 0)
	{
		printf("_SaveJPG�� ����ʧ��\n");
		return;
	}

	//��ȡͼƬ����ʱ��ǰ��һ������������ҳͷ����Ҫ������2��\r\n�ָ��
	char headBuff[1024] = { 0 };
	int recvLen = recv(socket, headBuff, 1024, 0);
	if (recvLen <= 0)
	{
		return;
	}

	char* pJpg = strstr(headBuff, "\r\n\r\n");  //����ҳͷȥ��
	if (!pJpg) return;


	pJpg += 4;  //ƫ�Ʒָ��

				//��ʣ��Ĳ���д�ļ�
				//1.�õ��ļ���
	string fileName = fileAddr.substr(fileAddr.find_last_of('/') + 1);
	if (fileName.empty()) return;

	fstream file;
	file.open(fileName, ios::out | ios::binary); //������д��

	file.write(pJpg, recvLen - (pJpg - headBuff)); //��ʣ�������д�ļ�

	while (1)
	{
		char recvBuff[1024] = { 0 };
		int recvLen = recv(socket, recvBuff, 1024, 0);
		if (recvLen <= 0)
			break;
		else
		{
			file.write(recvBuff, recvLen); //��ʣ�������д�ļ�
		}
	}

	file.close();

	//�ر��׽���
	closesocket(socket);
}


