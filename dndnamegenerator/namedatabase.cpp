#include "namedatabase.h"
#include "sqlite/sqlite3.h"
#include <string>
#include <iostream>

namedatabase::namedatabase() {
	rc = sqlite3_open("myDb.db", &namedb);
	if (!rc) {
	}
}

static int callback(void* data, int argc, char** argv, char** azColName) {
	namedatabase* nameDB = reinterpret_cast<namedatabase*>(data);
	if (nameDB->store_name(argv[0])) {
		return 0;
	}
	else {
		return 1;
	}
}

bool namedatabase::store_name(char* name) {
	std::string temp(name);
	outname = temp;
	return true;
}

std::string namedatabase::generate(std::string nationality, std::string gender, int length, int medieval, int aristocratic) {

	if (nationality == "chinese") {
		std::string personal = fetchname(nationality, "personal_one", gender, length, medieval, aristocratic) + fetchname(nationality, "personal_two", gender, length, medieval, aristocratic);
		std::string family = fetchname(nationality, "family", "any", length, medieval, aristocratic);
		return personal + " " + family;
	}

	std::string personal = fetchname(nationality, "personal", gender, length, medieval, aristocratic);
	std::string family = fetchname(nationality, "family", "any", length, medieval, aristocratic);
	return personal + " " + family;
}

std::string namedatabase::fetchname(std::string nationality, std::string type, std::string gender, int length, int medieval, int aristocratic) {

	std::string request = "SELECT * FROM " + nationality + " WHERE type = '" + type + "'";

	if (gender != "any") {
		request += " AND gender = '";
		request += gender;
		request += "'";
	}

	if (medieval == 1) {
		request += " AND medieval = 'true'";
	}
	if (aristocratic == 1) {
		request += " AND aristocratic = 'true'";
	}
	request += " ORDER BY RANDOM() LIMIT 1;";

	if (sqlite3_exec(namedb, request.c_str(), callback, this, &err)){
		return "DatabaseRequestFailureError";
	}
	else {
		return outname;
	}

}