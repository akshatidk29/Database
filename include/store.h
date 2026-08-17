#pragma once

extern int numDatabases;

bool checkDatabaseExistence(int id);
bool addNewDatabase(int id, std::string password);
bool authorizeDatabaseAccess(int id, std::string password);
bool changeDatabasePassword(int id, std::string previousPassword, std::string newPassword);

