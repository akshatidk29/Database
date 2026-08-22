#pragma once
#include "logger.h"
#include "transaction.h"

class Database{
private:
   Logger* logger;
   const int id;
   std::string password;

   bool access;

   void deleteEntry(const int& key);
   void writeEntry(const int& key, const std::string& value);
   void updateEntry(const int& key, const std::string& value);
   void readEntry(const int& key, std::string* value, const bool& printValue);

public:
   Database(const int& id, std::string& password, bool create);

   int  getId();
   void changePassword(std::string& previousPassword, std::string& newPassword);
   
   void startTransaction(Transaction* txn);
};