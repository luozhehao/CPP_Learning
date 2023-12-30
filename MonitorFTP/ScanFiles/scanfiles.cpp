


#include<stdio.h>
#include<io.h>
#include<iostream>
#include<string>

using namespace std;

int jpg_cnt = 0;

void findfile(string path, string mode)
{
	_finddata_t file;
	intptr_t HANDLE;
	string Onepath = path + mode;
	HANDLE = _findfirst(Onepath.c_str(), &file);
	if (HANDLE == -1L)
	{
		cout << "findfile, can not match the folder path" << endl;
		//system("pause");
	}
	else 
	{
		do {
			//判断是否有子目录  
			printf("findfile, file.attrib = %d, file.name = %s\n", file.attrib, file.name);
			if (file.attrib & _A_SUBDIR)
			{
				//判断是否为"."当前目录，".."上一层目录
				if ((strcmp(file.name, ".") != 0) && (strcmp(file.name, "..") != 0))
				{
					string newPath = path + "\\" + file.name;
					findfile(newPath, mode);
				}
			}
			else
			{
				cout <<"jpg: " << path << "\\" <<file.name << " " << endl;
				jpg_cnt++;
				printf("findfile, jpg_cnt = %d\n\n", jpg_cnt);
			}
		} while (_findnext(HANDLE, &file) == 0);
		_findclose(HANDLE);

	}

}

void findsubdir(string path, string mode)
{
	_finddata_t file;
	intptr_t HANDLE;
	string Onepath = path + mode;
	HANDLE = _findfirst(Onepath.c_str(), &file);
	if (HANDLE == -1L )
	{
		cout << "findsubdir, can not match the folder path" << endl;
		//system("pause");
	}
	else
	{
		do {
			//判断是否有子目录  
			printf("findsubdir, file.attrib = %d, file.name = %s \n\n", file.attrib, file.name);
			if ( (file.attrib & _A_SUBDIR) )
			{
				//判断是否为"."当前目录，".."上一层目录
				if ((strcmp(file.name, ".") != 0) && (strcmp(file.name, "..") != 0))
				{
					string newPath = path + "\\" + file.name;
					findsubdir(newPath, mode);
					mode = "\\.jpg";
					findfile(newPath, mode);
				}
			}
			else if(file.attrib == _A_ARCH)
			{
				//cout << "jpg: " << path << "\\" << file.name << " " << endl;
				string jpg_str = path + "\\" + file.name;
				cout << jpg_str << endl;
				cout << path << "\\" << file.name << " " << endl;
				jpg_cnt++;
				printf("file, jpg_cnt = %d\n\n", jpg_cnt);
			}


		} while ( _findnext(HANDLE, &file) == 0 );
		_findclose(HANDLE);

	}


	// subdir
	_finddata_t file_sub;
	intptr_t HANDLE_SUB;
	string Onepath_sub = path + "\\*";
	HANDLE_SUB = _findfirst(Onepath_sub.c_str(), &file_sub);
	if (HANDLE_SUB == -1L)
	{
		cout << "SUB can not match the folder path" << endl;
		//system("pause");
	}
	else
	{
		do {
			//判断是否有子目录  
			printf("file_sub.attrib = %d, file_sub.name = %s \n", file_sub.attrib, file_sub.name);
			if (file_sub.attrib & _A_SUBDIR)
			{
				//判断是否为"."当前目录，".."上一层目录
				if ((strcmp(file_sub.name, ".") != 0) && (strcmp(file_sub.name, "..") != 0))
				{

					string newPath = path + "\\" + file_sub.name;
					cout << "newPath: "<< newPath << endl;
					findsubdir(newPath, mode);
				}
			}
			else
			{
				cout << " SUB path: "<< path << "\\"<< file_sub.name << " " << endl;
				//cout << path << file_sub.name << " " << endl;
				//jpg_cnt++;
				//printf("sub jpg_cnt = %d\n\n", jpg_cnt);
			}
		} while (_findnext(HANDLE_SUB, &file_sub) == 0);
		_findclose(HANDLE_SUB);
	}

}


int main(int argc, char **argv)
{
	//string mode = "\\*.jpg";
	string mode = "\\*.jpg";
	string path = "O:\\Picture";
	findsubdir(path, mode);   //findfile
	//findfile(path, mode);   //findsubdir
	system("pause");
	return 0;
}