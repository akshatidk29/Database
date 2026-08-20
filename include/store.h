#pragma once
#include "return.h"

extern int numDatabases;



ReturnCode checkDatabaseExistence(const int& id);

ReturnCode addNewDatabase(const int& id, const std::string& password);


ReturnCode authorizeDatabaseAccess(const int& id, const std::string& password);

ReturnCode changeDatabasePassword(const int& id, const std::string& previousPassword, const std::string& newPassword);


ReturnCode deleteDatabaseEntry(const int& id, const int& key);

ReturnCode readDatabaseEntry(const int& id, const int& key, std::string* value);

ReturnCode writeDatabaseEntry(const int& id, const int& key, const std::string& value);

ReturnCode updateDatabaseEntry(const int& id, const int& key, const std::string& value);

