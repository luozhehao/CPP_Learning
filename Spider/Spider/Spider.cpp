
#include "Spider.h"


int main()
{
	//
	printf("*******************************************\n");
	printf("*                                         *\n");
	printf("*         ��ӭʹ���˹�������������        *\n");
	printf("*         �����ߣ��ٿ�����������ʦV1.0    *\n");
	printf("*         ��Ȩ����       ����ؾ�         *\n");
	printf("*                                         *\n");
	printf("*******************************************\n");
	

	//����һ���ļ��д��ͼƬ
	CreateDirectory(L"images", NULL);  //windows.h

	printf("������Ҫץȡ��URL��ַ��\n");
	string url;
	cin >> url;

	//ץȡ��վ������ͼƬ����(torrent, avi, mp4)
	StartCatch(url);




	return 0;
}


//URL���� -> ȡ��url -> url���� -> ���� ->  ������ҳ -> ������ҳ   -> 
//  ->  .avi��Ƶ ���� ͼƬ URL  -> �м�ֵ�� ���� ->URL����

bool StartCatch(string url)
{
	//ԭ��

	queue<string> q;
	q.push(url);

	while (!q.empty())
	{
		//ȡ����ַ
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

	cout << "����:  "<< m_host << "   " <<m_object << endl;


	return true;
}


//���캯��
CHttp::CHttp()
{
	//��ʼ������
	WSADATA wd;
	WSAStartup(MAKEWORD(2,2), &wd);

	//�����׽���
	m_socket = socket(AF_INET, SOCK_STREAM, 0);


}

//����
bool CHttp::Connect()
{
	// www.mmonly.cc
	// IP��ַ

	//����ת��ΪIP��ַ
	hostent* host = gethostbyname(m_host.c_str());
	if (host == NULL)
		return false;
	
	//����web������
	sockaddr_in sa;
	sa.sin_family = AF_INET;
	sa.sin_port = htons(80);
	memcpy(&sa.sin_addr, host->h_addr, 4);

	if (SOCKET_ERROR == connect(m_socket, (sockaddr*)&sa, sizeof(sockaddr)))
		return false;

	cout << "���ӳɹ�"<< endl;



	return true;
}


//  http://www.mmonly.cc/mmtp/nymn/306559.html
//  http://www.mmonly.cc
// http://www.mmonly.cc/mmtp/nymn/304552.html

//��ȡ��ҳ����
bool CHttp::GetHtml(string& html)
{
	//������Ϣ
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

	//�浽�ļ���
	fstream dataFile;   ////����һ���ļ������ڴ��html������dataFile.open("D:\\html.txt", std::ios::out);
	dataFile.open("N:\\html.txt", ios::app | ios::out);
	if (!dataFile)
	{
		printf("�ļ���ʧ��\n");
		return false;
	}

	//��������
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
//����������ʽ����ȡͼƬ����
//����ƥ��
//��������
//�����޸�
void CHttp::GetImagesUrlfromHtml(const string& html, set<string>& imageUrl)
{
	smatch mat;    //����ƥ���

	regex patten("\"http://(.*?\\.jpg)\"");

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
