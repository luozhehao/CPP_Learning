
#include <WinSock2.h>   //����ͷ�ļ�
#include <Windows.h>
#include <string>
#include <queue>
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <regex>        //c++ 11 ������ʽ
using namespace std;

#pragma comment(lib, "ws2_32.lib")   //������ļ�


//��ʼ�������
void InitNetwork();

//��url����ȡͼƬ
void SnapImageFromUrl(string url);

//��url�л�ȡ���������ļ���
void GetHostAddrByUrl(string url, string& hostAddr, string& fileAddr);

//��������
unsigned int  ConnectAddr(string hostAddr);

//ģ��httpЭ�飬���������������һ�ȡ��ҳԴ����
string GetHtmlCode(unsigned int socket, string hostAddr, string fileAddr);

//��html����ȡ��ͼƬ����
void GetImagesUrlfromHtml(const string& html, set<string>& imageUrl);

//��html����ȡ������
void GetUrlfromHtml(const string& html, set<string>& Url);

//��ȡ������ͼƬ
void SaveImage(const string& imageUrl);

//
void _SaveJPG(unsigned int socket, string hostAddr, string fileAddr);


//https://blog.csdn.net/NKU_Yang/article/details/109489127

