#pragma once
#include "return.h"

extern int numDatabases;



ReturnCode checkDatabaseExistence(int id);

ReturnCode addNewDatabase(int id, std::string password);


ReturnCode authorizeDatabaseAccess(int id, std::string password);

ReturnCode changeDatabasePassword(int id, std::string previousPassword, std::string newPassword);


ReturnCode deleteDatabaseEntry(int id, int key);

ReturnCode readDatabaseEntry(int id, int key, std::string& value);

ReturnCode writeDatabaseEntry(int id, int key, std::string& value);

ReturnCode updateDatabaseEntry(int id, int key, std::string& value);

