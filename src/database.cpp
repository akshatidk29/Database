#include <iostream>
#include <fstream>

#include "store.h"
#include "return.h"
#include "logger.h"
#include "database.h"

Database::Database(const int& id, std::string& password, bool create=false)
   : logger(nullptr), id(id), password(password), access(false){

   if(!create){
      ReturnCode check = authorizeDatabaseAccess(id, password);
      
      if(check == ReturnCode::SUCCESS){
         this->password = password;
         this->logger = new Logger(this->id);
         std::cout << "Authorized, giving access to database." << std::endl;
         access = true;
      }
      else{
         if(check == ReturnCode::FAILURE){
            std::cout << "Wrong credentials!" << std::endl;
         }
         else{
            std::cout << "Internal server error!" << std::endl;
         }
      }
   }
   else{
      if(password == ""){
         std::cout << "Password can't be empty!" << std::endl;
         return;
      }
      ReturnCode check = addNewDatabase(id, password);
      if(check == ReturnCode::SUCCESS){
         this->password = password;
         this->logger = new Logger(this->id);
         std::cout << "Database added!" << std::endl;
         access = true;
      }
      else if(check == ReturnCode::FAILURE){
         std::cout << "Database with same ID already exists!" << std::endl;
      }else{
         std::cout << "Internal server error!" << std::endl; 
      }
   }
}

int Database::getId(){
  
   if(!(this->access)){
      std::cout << "Access denied!" << std::endl;
      return -1;
   }

   return this->id;
}

void Database::changePassword(std::string& previousPassword, std::string& newPassword){
   
   if(!(this->access)){
      std::cout << "Access denied!" << std::endl;
      return;
   }

   if(newPassword == ""){
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


void Database::readEntry(const int& key, std::string* value, const bool& printValue=false){
  
   if(!(this->access)){
      std::cout << "Access denied!" << std::endl;
      return;
   }

   ReturnCode check = readDatabaseEntry(this->id, key, value);

   if(check == ReturnCode::FAILURE){
      std::cout << "Key not found!" << std::endl;
   }
   else if(check != ReturnCode::SUCCESS){
      std::cout << "Internal server error!" << std::endl;
   }else if(printValue){
      std::cout << key << " : " << *(value) << std::endl;
   }
}

void Database::writeEntry(const int& key, const std::string& value){
  
   if(!(this->access)){
      std::cout << "Access denied!" << std::endl;
      return;
   }
       
   ReturnCode check = writeDatabaseEntry(this->id, key, value);

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

void Database::updateEntry(const int& key, const std::string& value){
  
   if(!(this->access)){
      std::cout << "Access denied!" << std::endl;
      return;
   }
    
   ReturnCode check = updateDatabaseEntry(this->id, key, value);

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

void Database::deleteEntry(const int& key){
  
   if(!(this->access)){
      std::cout << "Access denied!" << std::endl;
      return;
   }
   
   ReturnCode check = deleteDatabaseEntry(this->id, key);

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