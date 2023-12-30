
#include <WinSock2.h>   //网络头文件
#include <Windows.h>
#include <string>
#include <queue>
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <regex>        //c++ 11 正则表达式
using namespace std;

#pragma comment(lib, "ws2_32.lib")   //网络库文件


//初始化网络库
void InitNetwork();

//从url中爬取图片
void SnapImageFromUrl(string url);

//从url中获取主机名和文件名
void GetHostAddrByUrl(string url, string& hostAddr, string& fileAddr);

//连接主机
unsigned int  ConnectAddr(string hostAddr);

//模拟http协议，发送至主机，并且获取网页源代码
string GetHtmlCode(unsigned int socket, string hostAddr, string fileAddr);

//从html中提取出图片链接
void GetImagesUrlfromHtml(const string& html, set<string>& imageUrl);

//从html中提取出链接
void GetUrlfromHtml(const string& html, set<string>& Url);

//获取并保存图片
void SaveImage(const string& imageUrl);

//
void _SaveJPG(unsigned int socket, string hostAddr, string fileAddr);


//https://blog.csdn.net/NKU_Yang/article/details/109489127

