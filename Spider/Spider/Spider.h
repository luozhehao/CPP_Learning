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

#pragma comment(lib, "ws2_32.lib")  //lib��׺��β�Ľ�����̬��,��̬��dll


class CHttp
{
private:
	string m_host;   //����
	string m_object;  //��Դ��
	SOCKET m_socket;

public:
	CHttp();					 //���캯��
	bool AnalyseURL(string url);  //����URL
	bool Connect();			 	 //���� 
	bool GetHtml(string& html);
	void CHttp::GetImagesUrlfromHtml(const string& html, set<string>& imageUrl);


};

//��ʼץȡ
bool StartCatch(string url);