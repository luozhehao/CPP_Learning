
#include "Spider.h"


int main()
{
	//
	printf("*******************************************\n");
	printf("*                                         *\n");
	printf("*         欢迎使用人工智能网络爬虫        *\n");
	printf("*         开发者：顿开教育里奇老师V1.0    *\n");
	printf("*         版权所有       盗版必究         *\n");
	printf("*                                         *\n");
	printf("*******************************************\n");
	

	//创建一个文件夹存放图片
	CreateDirectory(L"images", NULL);  //windows.h

	printf("请输入要抓取的URL地址：\n");
	string url;
	cin >> url;

	//抓取网站的所有图片链接(torrent, avi, mp4)
	StartCatch(url);




	return 0;
}


//URL队列 -> 取出url -> url解析 -> 联网 ->  下载网页 -> 解析网页   -> 
//  ->  .avi视频 种子 图片 URL  -> 有价值的 下载 ->URL队列

bool StartCatch(string url)
{
	//原理

	queue<string> q;
	q.push(url);

	while (!q.empty())
	{
		//取出网址
		string currentUrl = q.front();
		q.pop();

		CHttp http;
		if (false == http.AnalyseURL(currentUrl))
			return false;

		if(false == http.Connect())
			return false;

		string html;
		http.GetHtml(html);
		cout << html << endl;


	}


	return true;

}


bool CHttp::AnalyseURL(string url)
{
	//https://unsplash.com/ko/%EC%82%AC%EC%A7%84/%EB%8B%A4%EB%A6%AC-%EC%98%86-%EA%B1%B0%EB%A6%AC%EB%A5%BC-%EB%94%B0%EB%9D%BC-%EC%9E%90%EC%A0%84%EA%B1%B0%EB%A5%BC-%ED%83%80%EA%B3%A0-%EC%9E%88%EB%8A%94-%EB%82%A8%EC%9E%90-pYffZ7Ak_zE
	// https://500px.com.cn/community/photo-details/a72aa129768d4c88b9ee80836905f741
	
	//  http://www.mmonly.cc/mmtp/nymn/306559.html
	//  http://www.mmonly.cc
	if (url.length() <= 7)
		return false;

	if(-1 == url.find("http://"))
		return false;

	int pos = url.find("/", 7);
	if (pos == -1)
	{
		m_host = url.substr(7);
		m_object = "/";
	}
	else
	{
		m_host = url.substr(7, pos - 7);
		m_object = url.substr(pos);
	}

	cout << "域名:  "<< m_host << "   " <<m_object << endl;


	return true;
}


//构造函数
CHttp::CHttp()
{
	//初始化网络
	WSADATA wd;
	WSAStartup(MAKEWORD(2,2), &wd);

	//创建套接字
	m_socket = socket(AF_INET, SOCK_STREAM, 0);


}

//联网
bool CHttp::Connect()
{
	// www.mmonly.cc
	// IP地址

	//域名转化为IP地址
	hostent* host = gethostbyname(m_host.c_str());
	if (host == NULL)
		return false;
	
	//连接web服务器
	sockaddr_in sa;
	sa.sin_family = AF_INET;
	sa.sin_port = htons(80);
	memcpy(&sa.sin_addr, host->h_addr, 4);

	if (SOCKET_ERROR == connect(m_socket, (sockaddr*)&sa, sizeof(sockaddr)))
		return false;

	cout << "连接成功"<< endl;



	return true;
}


//  http://www.mmonly.cc/mmtp/nymn/306559.html
//  http://www.mmonly.cc
// http://www.mmonly.cc/mmtp/nymn/304552.html

//获取网页数据
bool CHttp::GetHtml(string& html)
{
	//发送信息
	string info;
	info = info + "GET " + m_object + " HTTP/1.1\r\n";
	info = info + "Host: " + m_host + "\r\n";
	info = info + "Connection: Close\r\n";
	//info = info + "User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/119.0.0.0 Safari/537.36 OPR/105.0.0.0\r\n";
	info = info + "User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/120.0.0.0 Safari/537.36 Edg/120.0.0.0\r\n";

	info = info + "\r\n";

	cout << "info:"<< endl;
	cout << info << endl;

	if(SOCKET_ERROR==send(m_socket, info.c_str(), info.length(), 0))
		return false;

	//存到文件里
	fstream dataFile;   ////创建一个文件，用于存放html的内容dataFile.open("D:\\html.txt", std::ios::out);
	dataFile.open("N:\\html.txt", ios::app | ios::out);
	if (!dataFile)
	{
		printf("文件打开失败\n");
		return false;
	}

	//接收数据
	char ch = 0;
	while (recv(m_socket, &ch, sizeof(char), 0))
	{
		html = html + ch;
		dataFile << ch;
	}

	dataFile.close();



	return true;
}


//  https://blog.csdn.net/NKU_Yang/article/details/109489127
//利用正则表达式，提取图片链接
//用于匹配
//用于搜索
//用于修改
void CHttp::GetImagesUrlfromHtml(const string& html, set<string>& imageUrl)
{
	smatch mat;    //用于匹配的

	regex patten("\"http://(.*?\\.jpg)\"");

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
