#include <iostream>
#include <fstream>

#include "store.h"
#include "return.h"
#include "database.h"

Database::Database(int id)
   : id(id), password(""){

   ReturnCode check = checkDatabaseExistence(id);

   if(check == ReturnCode::FAILURE){
      std::cout << "Database creation instantiated, set the password to get started!" << std::endl;
   }
   else{
      if(check == ReturnCode::SUCCESS){
         std::cout << "Database with same id already exists!" << std::endl;
      }else{
         std::cout << "Internal server error!" << std::endl;
      }
   } 
}

Database::Database(int id, std::string password)
   : id(id), password(""){
   
   ReturnCode check = authorizeDatabaseAccess(id, password);

   if(check == ReturnCode::SUCCESS){
      this->password = password;
      std::cout << "Authorized, Giving Access to Database." << std::endl;
   }
   else{
      if(check == ReturnCode::FAILURE){
      std::cout << "Wrong Credentials!" << std::endl;
      }
      else{
         std::cout << "Internal server error!" << std::endl;
      }
   }
}

int Database::getId(){
   return this->id;
}

void Database::setPassword(std::string password){
   if(password == ""){
      std::cout << "Password can't be empty!" << std::endl;
      return;
   }
   if(this->password == ""){
      ReturnCode check = addNewDatabase(id, password);
      if(check == ReturnCode::SUCCESS){
         this->password = password;
         std::cout << "Password set successfully for the database!" << std::endl;
      }
      else if(check == ReturnCode::FAILURE){
         std::cout << "Database with same ID already exists!" << std::endl;
      }else{
         std::cout << "Internal server error!" << std::endl; 
      }
   }else{
      std::cout << "Password already created, use changeDatabasePassword." << std::endl;
   }
}

void Database::changePassword(std::string previousPassword, std::string newPassword){

   if(this->password == ""){
      std::cout << "Please Set Password First, using setPassword!" << std::endl;
      return;
   }  
   else if(newPassword == ""){
      std::cout << "Password can't be empty!" << std::endl;
      return;
   }

   ReturnCode check  = changeDatabasePassword(this->id, previousPassword, newPassword);

   if(check == ReturnCode::SUCCESS){
      this->password = newPassword;
      std::cout << "Password changed successfully!" << std::endl;
   }
   else if(check == ReturnCode::WROND_CREDENTIALS){
      std::cout << "Wrong credentials!" << std::endl;
   }
   else{
      std::cout << "Internal server error!" << std::endl;
   }
}

std::string Database::read(int key){

   if(this->password == ""){
      std::cout << "Please Set Password First, using setPassword!" << std::endl;
      return "";
   }

   std::string value = "";
   ReturnCode check = readDatabase(this->id, key, value);

   if(check == ReturnCode::FAILURE){
      std::cout << "Key not found!" << std::endl;
   }
   else if(check != ReturnCode::SUCCESS){
      std::cout << "Internal server error!" << std::endl;
   }
   return value;
}

void Database::write(int key, std::string value){
      
   if(this->password == ""){
      std::cout << "Please Set Password First, using setPassword!" << std::endl;
      return;
   }

   ReturnCode check = writeDatabase(this->id, key, value);

   if(check == ReturnCode::SUCCESS){
      return;
   }
   else if(check == ReturnCode::KEY_ALREADY_EXIST){
      std::cout << "Key already exists!" << std::endl; 
   }
   else{
      std::cout << "Internal server error!" << std::endl;
   }
}

void Database::update(int key, std::string value){
      
   if(this->password == ""){
      std::cout << "Please Set Password First, using setPassword!" << std::endl;
      return;
   }

   ReturnCode check = updateDatabase(this->id, key, value);

   if(check == ReturnCode::SUCCESS){
      return;
   }
   else if(check == ReturnCode::KEY_NOT_FOUND){
      std::cout << "Key not found!" << std::endl;
   }
   else{
      std::cout << "Internal server error!" << std::endl;
   }
}