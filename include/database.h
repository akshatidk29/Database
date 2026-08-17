#pragma once

class Database{
private:
   int id;
   std::string password;

public:
   Database(int id);
   Database(int id, std::string password);

   void setDatabasePassword(std::string password);
   
   std::string read(int key);

   int write(int key, std::string value);
};