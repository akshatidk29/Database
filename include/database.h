#pragma once

class Database{
private:
   int id;
   std::string password;

public:
   Database(int id, std::string password, bool create);

   int  getId();
   void changePassword(std::string previousPassword, std::string newPassword);
   
   void deleteEntry(int key);
   std::string readEntry(int key);
   void writeEntry(int key, std::string value);
   void updateEntry(int key, std::string value);
};