#pragma once
#include "transaction.h"

class Database{
private:
   int id;
   std::string password;

   void deleteEntry(int key);
   void readEntry(int key, std::string* value);
   void writeEntry(int key, const std::string& value);
   void updateEntry(int key, const std::string& value);

public:
   Database(int id, std::string password, bool create);

   int  getId();
   void changePassword(std::string previousPassword, std::string newPassword);
   
   void startTransaction(Transaction* txn);
};