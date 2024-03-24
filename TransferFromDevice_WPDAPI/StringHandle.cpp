

#include "stdafx.h"

#include "StringHandle.h"



void wchar2wstring_test()
{

	// ������һ�� wchar_t �ַ�������
	const wchar_t* wcharStrings[] = { L"Hello", L"World", L"Goodbye" };

	// ʹ�� std::vector �洢 wstring ����
	std::vector<std::wstring> wstringArray;

	// �� wchar_t �ַ������ת��Ϊ wstring ����ӵ� vector ��
	for (const wchar_t* wstr : wcharStrings) {
		wstringArray.push_back(std::wstring(wstr));
	}

	// ��� wstring ��������
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
	// ����һ���ַ�������
	std::vector<std::wstring> originalArray = { L"Hello", L"World", L"C++", L"is", L"awesome!" };

	// ���ַ�������д���ļ�
	writeArrayToFile(originalArray, L"warray.txt");

	// ��ȡ�ļ��е��ַ�������
	std::vector<std::wstring> newArray;
	readArrayFromFile(newArray, L"warray.txt");

	// ��������������
	std::cout << "New array:" << std::endl;
	for (const auto& str : newArray) 
	{
		std::wcout << str << std::endl;
	}

	return 0;
}




