#include <string>
#include <Windows.h>

std::string toStdString(wchar_t* string) {
	std::wstring temp(string);
	std::string res(temp.begin(), temp.end());
	return res;
}

LPCWSTR toLPCWSTR(std::string string) {
	std::wstring temp(string.begin(), string.end());
	LPCWSTR res = temp.c_str();
	return res;
}