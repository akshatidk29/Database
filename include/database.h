#pragma once

class Database{
private:
   int id;
   std::string password;

public:
   Database(int id);
   Database(int id, std::string password);

   int  getId();
   void setPassword(std::string password);
   void changePassword(std::string previousPassword, std::string newPassword);
   
   std::string read(int key);
   void write(int key, std::string value);
   void update(int key, std::string value);
};