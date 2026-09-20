#pragma once

#include "storage/index.h"
#include "storage/logger.h"
#include "core/transaction.h"

class Database{
private:
   const int id;
   Index* index;
   Logger* logger;
   std::string password;

   bool access;

   void deleteEntry(const int& key);
   void writeEntry(const int& key, const std::string& value);
   void updateEntry(const int& key, const std::string& value);
   void readEntry(const int& key, std::string* value, const bool& printValue);

   void compactDatabase();

public:
   Database(const int& id, std::string& password, bool create);
   ~Database();

   int  getId();
   void changePassword(std::string& previousPassword, std::string& newPassword);
   
   void startTransaction(Transaction* txn);
};