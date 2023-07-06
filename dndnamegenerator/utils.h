#include <string>
#pragma once

std::string toStdString(wchar_t* string);
LPCWSTR toLPCWSTR(std::string string);
