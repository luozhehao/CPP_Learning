

#include "stdafx.h"

#include "StringHandle.h"



void wchar2wstring_test()
{

	// 假设有一个 wchar_t 字符串数组
	const wchar_t* wcharStrings[] = { L"Hello", L"World", L"Goodbye" };

	// 使用 std::vector 存储 wstring 对象
	std::vector<std::wstring> wstringArray;

	// 将 wchar_t 字符串逐个转换为 wstring 并添加到 vector 中
	for (const wchar_t* wstr : wcharStrings) {
		wstringArray.push_back(std::wstring(wstr));
	}

	// 输出 wstring 数组内容
	for (const std::wstring& wstr : wstringArray) {
		std::wcout << "wstr:" << wstr << std::endl;
	}
}


void writeArrayToFile(const std::vector<std::wstring>& array, const std::wstring& filename) 
{
	std::wofstream file(filename, std::wofstream::app);
	if (file.is_open()) 
	{
		for (const auto& str : array) 
		{
			file << str << std::endl;
		}
		file.close();
		std::cout << "Array successfully written to file." << std::endl;
	}
	else 
	{
		std::cerr << "Unable to open file for writing." << std::endl;
	}
}

void readArrayFromFile(std::vector<std::wstring>& array, const std::wstring& filename) 
{
	std::wifstream file(filename);
	if (file.is_open()) 
	{
		std::wstring str;
		while (std::getline(file, str)) 
		{
			array.push_back(str);
		}
		file.close();
		std::cout << "Array successfully read from file." << std::endl;
	}
	else {
		std::cerr << "Unable to open file for reading." << std::endl;
	}
}

int file_read_write_test() 
{
	// 定义一个字符串数组
	std::vector<std::wstring> originalArray = { L"Hello", L"World", L"C++", L"is", L"awesome!" };

	// 将字符串数组写入文件
	writeArrayToFile(originalArray, L"warray.txt");

	// 读取文件中的字符串数组
	std::vector<std::wstring> newArray;
	readArrayFromFile(newArray, L"warray.txt");

	// 输出新数组的内容
	std::cout << "New array:" << std::endl;
	for (const auto& str : newArray) 
	{
		std::wcout << str << std::endl;
	}

	return 0;
}




