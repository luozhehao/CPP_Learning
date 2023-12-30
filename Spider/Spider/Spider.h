//#pragma once


#include <stdio.h>
#include <Windows.h>

#include <iostream>
#include <string>
#include <queue>
#include <set> 
#include<regex>
#include <fstream>

using namespace std;

#pragma comment(lib, "ws2_32.lib")  //lib后缀结尾的叫做静态库,动态库dll


class CHttp
{
private:
	string m_host;   //域名
	string m_object;  //资源名
	SOCKET m_socket;

public:
	CHttp();					 //构造函数
	bool AnalyseURL(string url);  //解析URL
	bool Connect();			 	 //联网 
	bool GetHtml(string& html);
	void CHttp::GetImagesUrlfromHtml(const string& html, set<string>& imageUrl);


};

//开始抓取
bool StartCatch(string url);