#include "sqlite/sqlite3.h"
#include <string>

#pragma once
class namedatabase
{
	char* err;
	sqlite3* namedb;
	sqlite3_stmt* stmt;
	int rc;
public:
	namedatabase();
	std::string fetchname(std::string nationality, std::string type, std::string gender, int length = -1, int medieval = 0, int aristocratic = 0);
	std::string generate(std::string nationality, std::string gender, int length = -1, int medieval = 0, int aristocratic = 0);
	bool store_name(char* name);
	std::string outname;
};

