#pragma once

#include "CommonFunctions.h"





void wchar2wstring_test();

int file_read_write_test();

void writeArrayToFile(const std::vector<std::wstring>& array, const std::wstring& filename);

void readArrayFromFile(std::vector<std::wstring>& array, const std::wstring& filename);


