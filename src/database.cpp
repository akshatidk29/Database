#include<iostream>
#include<fstream>
#include"database.h"
#include"store.h"

Database::Database(int id){
   this->id = id;
   this->password = "";
   std::cout << "Database Creation Instantiated, Set the Password to get Started!" << std::endl;
}

Database::Database(int id, std::string password){
   bool check = authorizeDatabaseAccess(id, password);
   if(check){
      this->id = id;
      this->password = password;
      std::cout << "Authorized, Giving Access to Database " << id << std::endl;
   }else{
      std::cout << "Wrong Credentials!\n";
   }
}

void Database::setDatabasePassword(std::string password){
   if(password == ""){
      std::cout << "Password cant be empty." << std::endl;
      return;
   }
   if(this->password == ""){
      bool check = addNewDatabase(id, password);
      if(check){
         this->password = password;
         std::cout << "Password Set for the Database with id " << this->id << std::endl;
      }
   }
}

std::string Database::read(int key){

   if(this->password == ""){
      std::cout << "Please Set Password First, using setDatabasePassword!\n";
      return "";
   }

   std::string databasePath = "../data" + std::to_string(this->id) + ".db";
   std::ifstream databaseFileIn(databasePath);

   std::string line;

   while(std::getline(databaseFileIn, line)){
      int pos = line.find(':');
      if(pos == std::string::npos)
         continue;
      
      std::string presentKey = line.substr(0, pos);
      if(presentKey == std::to_string(key)){
         std::string value = line.substr(pos);
         databaseFileIn.close();
         return value;
      }

   }

   databaseFileIn.close();
   std::cout << "Not Found!\n";
   return "";
}

int Database::write(int key, std::string value){
      
   if(this->password == ""){
      std::cout << "Please Set Password First, using setDatabasePassword!\n";
      return -1;
   }

   std::string databasePath = "../data" + std::to_string(this->id) + ".db";

   std::ofstream databaseFileOut(databasePath);

   databaseFileOut << key << ":" << value << std::endl;

   return key;
}