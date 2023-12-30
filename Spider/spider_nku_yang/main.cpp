


#include "main.h"



int main(int argc, char** argv[])
{
	InitNetwork();
	//URL地址可以根据自己喜好随意更改，这里只是一个示例
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
	wVersionRequested = MAKEWORD(2, 2);//规定使用的协议版本

	err = WSAStartup(wVersionRequested, &wsaData);
	if (err != 0)
	{
		return;
	}
}

void SnapImageFromUrl(string url)
{
	//采用广度遍历的方式，爬网站下所有的图片
	queue<string> queUrls;
	queUrls.push(url);

	map<string, int> tempMap;   //用于记录已访问过的网址

	while (!queUrls.empty())
	{
		try {
			string curUrl = queUrls.front();   //取出队列中最开始被放进去的元素
			queUrls.pop();

			//自行研究
			if (curUrl.size() > 256) continue;

			//将已经访问过的放到map中
			tempMap[curUrl]++;

			printf("当前处理的：%d %s\n", queUrls.size(), curUrl.c_str());

			//将curUrl下的图片全部爬下来
			string hostAddr, fileAddr;
			SOCKET socket = 0;
			//获取主机名和文件名
			GetHostAddrByUrl(curUrl, hostAddr, fileAddr);

			if ((socket = ConnectAddr(hostAddr)) == 0)
			{
				printf("连接主机失败 : %s\n", hostAddr);
			}

			cout << "hostAddr: " << hostAddr << endl;
			cout << "fileAddr: " << fileAddr << endl;


			//获取网页源代码
			string html = GetHtmlCode(socket, hostAddr, fileAddr);
			if (!html.empty())
			{
				cout << "html:" << endl;
				cout <<  html << endl;

				//提取出网页中的图片链接
				set<string> imageUrl;
				GetImagesUrlfromHtml(html, imageUrl);

				int ret = imageUrl.empty();
				int size = imageUrl.size();
				printf("\n\nimageUrl, ret = %d, size = %d\n\n", ret, size);

				for (set<string>::iterator it = imageUrl.begin(); it != imageUrl.end(); it++)
				{
					printf("for iterator it:\n");
					printf("%s\n", (*it).c_str());
					SaveImage(*it);   //去弄个路径
				}

				printf("SnapImageFromUrl ...002, html:\n");
				//提取当前网页中的所有链接,并且将其放到队列中
				set<string> urls;
				GetUrlfromHtml(html, urls);

				int ret_url = urls.empty();
				int size_url = urls.size();
				printf("\n\n urls, ret_url = %d, size_url = %d\n\n", ret_url, size_url);

				for (set<string>::iterator it = urls.begin(); it != urls.end(); it++)
				{
					//printf("%s\n", (*it).c_str());
					//将提取出来的链接放到队列中,放进去之前，需要判断，是否访问过
					if (tempMap[(*it)] == 0)  //该网址没有被访问过
					{
						queUrls.push((*it));
						printf("queUrls it:\n");
						printf("%s\n", (*it).c_str());
						
					}
					else
					{
						printf("%s 已访问过\n", (*it).c_str());
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
	//模拟http协议
	string reqInfo;
	reqInfo = reqInfo + "GET " + fileAddr + " HTTP/1.1\r\n";
	reqInfo = reqInfo + "Host: " + hostAddr + "\r\n";
	reqInfo = reqInfo + "Connection: Close\r\n";
	reqInfo = reqInfo + "User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/119.0.0.0 Safari/537.36 OPR/105.0.0.0\r\n";
	reqInfo = reqInfo + "\r\n";

	cout << "reqInfo:\n"<< reqInfo << endl;

	//将模拟的http协议发送给主机
	int sendLen = send(socket, reqInfo.c_str(), reqInfo.size(), 0);
	if (sendLen <= 0)
		return "";

	//服务解析http协议之后，给我发来网页的源代码,接收它
	string htmlCode;
	while (1)
	{
		char recvBuff[1024] = { 0 };
		int recvLen = recv(socket, recvBuff, 1024, 0);
		if (recvLen <= 0)
			break;
		else
		{
			htmlCode += recvBuff;   //将网页源代码累加
		}
	}
	//数据导出层excel(com)

	//printf("%s\n", htmlCode.c_str());
	//....

	closesocket(socket);

	return htmlCode;
}



unsigned int ConnectAddr(string hostAddr)
{
	//创建套接字
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if (INVALID_SOCKET == sock)
		return INVALID_SOCKET;

	//有主机地址获取对应的IP地址
	hostent* pHost = gethostbyname(hostAddr.c_str());
	if (pHost == 0)
		return 0;

	//准备要连接的IP地址
	sockaddr_in addr;
	addr.sin_family = AF_INET;   //地址族域
	addr.sin_port = htons(80);   //端口号, h(host) to n(net) s(short), 将主机字节序转换成网络字节序
	memcpy(&(addr.sin_addr.S_un.S_addr), pHost->h_addr, 4);   //ip地址拷贝

															  //连接服务器
	if (SOCKET_ERROR == connect(sock, (sockaddr*)&addr, sizeof(sockaddr)))
		return 0;

	return sock;
}


void GetHostAddrByUrl(string url, string& hostAddr, string& fileAddr)
{
	//strstr();   //搜索字符串函数
	//strtok();   //分割字符串函数
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
	smatch mat;    //用于匹配的

	regex patten("a href=\"https://(www..*?\\.html)\"");

	//在一个范围内进行搜索，将搜索到的结果，匹配到smatch中
	string::const_iterator beginIter = html.begin();
	string::const_iterator endIter = html.end();
	while (regex_search(beginIter, endIter, mat, patten))
	{
		//找到了匹配项, 其结果在mat中
		string url(mat[1].first, mat[1].second);

		Url.insert(url);

		beginIter = mat[0].second;   //修改搜索的起始位置
	}
}



//利用正则表达式，提取图片链接
//用于匹配
//用于搜索
//用于修改
void GetImagesUrlfromHtml(const string& html, set<string>& imageUrl)
{
	smatch mat;    //用于匹配的

	//regex patten("\"http://(.*?\\.jpg)\"");
	regex patten("\"https://(.*?\\.jpg)\"");

	//在一个范围内进行搜索，将搜索到的结果，匹配到smatch中
	string::const_iterator beginIter = html.begin();
	string::const_iterator endIter = html.end();
	while (regex_search(beginIter, endIter, mat, patten))
	{
		//找到了匹配项, 其结果在mat中
		string url(mat[1].first, mat[1].second);

		imageUrl.insert(url);

		beginIter = mat[0].second;   //修改搜索的起始位置
	}
}


void SaveImage(const string& imageUrl)
{
	//1.获取主机名和文件名
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
	//模拟http协议
	string reqInfo = "GET " + fileAddr + " HTTP/1.1\r\n" +
		"HOST:" + hostAddr + "\r\n" +
		"Connection:Close\r\n\r\n";

	//将模拟的http协议发送给主机
	int sendLen = send(socket, reqInfo.c_str(), reqInfo.size(), 0);
	if (sendLen <= 0)
	{
		printf("_SaveJPG， 发送失败\n");
		return;
	}

	//获取图片码流时，前面一部分数据是网页头，需要丢弃，2个\r\n分割的
	char headBuff[1024] = { 0 };
	int recvLen = recv(socket, headBuff, 1024, 0);
	if (recvLen <= 0)
	{
		return;
	}

	char* pJpg = strstr(headBuff, "\r\n\r\n");  //将网页头去掉
	if (!pJpg) return;


	pJpg += 4;  //偏移分割符

				//将剩余的部分写文件
				//1.得到文件名
	string fileName = fileAddr.substr(fileAddr.find_last_of('/') + 1);
	if (fileName.empty()) return;

	fstream file;
	file.open(fileName, ios::out | ios::binary); //二进制写入

	file.write(pJpg, recvLen - (pJpg - headBuff)); //将剩余的数据写文件

	while (1)
	{
		char recvBuff[1024] = { 0 };
		int recvLen = recv(socket, recvBuff, 1024, 0);
		if (recvLen <= 0)
			break;
		else
		{
			file.write(recvBuff, recvLen); //将剩余的数据写文件
		}
	}

	file.close();

	//关闭套接字
	closesocket(socket);
}


