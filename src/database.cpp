#include<iostream>
#include<fstream>
#include"database.h"
#include"store.h"

Database::Database(int id){
   this->id = id;
   this->password = -1;
}

Database::Database(int id, int password){
   bool check = authorizeDatabase(id, password);
   if(check){
      this->id = id;
      this->password = password;
   }else{
     std::cout << "Wrong Credentials!\n";
   }
}

void Database::setPassword(int previousPassword, int newPassword){
   if(this->password != -1){
      changePassword(this->id, previousPassword, newPassword);
   }else{
      bool check = addDatabase(id, password);
      if(check)
         this->password = password;
   }
}

std::string Database::read(int key){

   if(this->password == -1){
      std::cout << "Please Set Password First, using setPassword!\n";
      return "-1";
   }

   std::string databasePath = "../data" + std::to_string(this->id) + ".db";
   std::ifstream databaseFile(databasePath);

   std::string line;

   while(std::getline(databaseFile, line)){
      int pos = line.find(':');
      if(pos == std::string::npos)
         continue;
      
      std::string presentKey = line.substr(0, pos);
      if(presentKey == std::to_string(key)){
         std::string value = line.substr(pos);
         databaseFile.close();
         return value;
      }

   }

   databaseFile.close();
   std::cout << "Not Found!\n";
   return "-1";
}

int Database::write(int key, int value){
      
   if(this->password == -1){
      std::cout << "Please Set Password First, using setPassword!\n";
      return -1;
   }

   std::string databasePath = "../data" + std::to_string(this->id) + ".db";

   std::ofstream databaseFile(databasePath);

   databaseFile << key << ":" << value << std::endl;

   return key;
}