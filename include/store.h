#pragma once
#include "return.h"

extern int numDatabases;



ReturnCode checkDatabaseExistence(int id);

ReturnCode addNewDatabase(int id, std::string password);


ReturnCode authorizeDatabaseAccess(int id, std::string password);

ReturnCode changeDatabasePassword(int id, std::string previousPassword, std::string newPassword);


ReturnCode readDatabase(int id, int key, std::string& value);

ReturnCode writeDatabase(int id, int key, std::string value);

ReturnCode updateDatabase(int id, int key, std::string value);