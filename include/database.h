#pragma once

class Database{
private:
   int id;
   int password;

public:
   Database(int id);
   Database(int id, int password);

   void setPassword(int previousPassword, int newPassword);
   
   std::string read(int key);
   int write(int key, int val);
};