#include <iostream>
#include <fstream>

#include "store.h"
#include "index.h"
#include "return.h"
#include "logger.h"
#include "database.h"

Database::Database(const int& id, std::string& password, bool create=false)
   : id(id), password(password), access(false), logger(nullptr), index(nullptr){

   if(!create){
      ReturnCode check = authorizeDatabaseAccess(id, password);
      
      if(check == ReturnCode::SUCCESS){
         this->password = password;
         this->logger = new Logger(this->id);
         this->index = new Index(this->id);
         if(this->logger->getStatus() && this->index->getStatus()){
            std::cout << "Authorized, giving access to database." << std::endl;
            access = true;
         }else{
            std::cout << "Internal server error!" << std::endl;
         }
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
         this->index = new Index(this->id);
         
         if(this->logger->getStatus() && this->index->getStatus()){
            std::cout << "Database added!" << std::endl;
            access = true;
         }else{
            std::cout << "Internal server error!" << std::endl;
         }
      }
      else if(check == ReturnCode::FAILURE){
         std::cout << "Database with same ID already exists!" << std::endl;
      }else{
         std::cout << "Internal server error!" << std::endl; 
      }
   }
}

Database::~Database() {
   delete logger;
   delete index;
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

   ReturnCode idxCheck = this->index->readIndexEntry(key, value);

   if(idxCheck == ReturnCode::FAILURE){
      std::cout << "Key not found!" << std::endl;
   }
   else if(idxCheck != ReturnCode::SUCCESS){
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
   ReturnCode idxCheck = this->index->writeIndexEntry(key, value);    
   ReturnCode dbCheck = writeDatabaseEntry(this->id, key, value);
}

void Database::updateEntry(const int& key, const std::string& value){
  
   if(!(this->access)){
      std::cout << "Access denied!" << std::endl;
      return;
   }
   
   ReturnCode idxCheck = this->index->updateIndexEntry(key, value);
   ReturnCode dbCheck = updateDatabaseEntry(this->id, key, value);
}

void Database::deleteEntry(const int& key){
  
   if(!(this->access)){
      std::cout << "Access denied!" << std::endl;
      return;
   }
   ReturnCode idxCheck = this->index->deleteIndexEntry(key);
   ReturnCode dbCheck = deleteDatabaseEntry(this->id, key);
}