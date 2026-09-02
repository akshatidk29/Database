#include <string>
#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <unordered_map>

#include "utils/return.h"
#include "utils/method.h"
#include "storage/index.h"

Index::Index(const int& id)
   : id(id), status(false) {

   ReturnCode check = buildIndex();
   if(check == ReturnCode::SUCCESS){
      this->status = true;
   }
}

bool Index::getStatus(){
   return this->status;
}

ReturnCode Index::buildIndex(){

   std::string databaseLogName = std::to_string(id) + ".log";
   std::string databaseLogPath =  "data/" + std::to_string(id) + "/" + databaseLogName;

   std::ifstream databaseLogFileIn(databaseLogPath);
   if(!databaseLogFileIn.is_open()){
      return ReturnCode::LOG_FILE_ERROR;
   }

   bool exists = false;
   std::string line;

   while(std::getline(databaseLogFileIn, line)){
      
      std::size_t colonPos1 = line.find(':');
      if(colonPos1 == std::string::npos){
         databaseLogFileIn.close();
         return ReturnCode::DATABASE_FILE_CORRUPT_ERROR;
      }

      std::size_t colonPos2 = line.find(':', colonPos1 + 1);
      if(colonPos2 == std::string::npos){
         databaseLogFileIn.close();
         return ReturnCode::DATABASE_FILE_CORRUPT_ERROR;
      }

      Method method = getMethod(line.substr(colonPos1 + 1, colonPos2 - colonPos1 - 1));

      if(method == Method::WRITE || method == Method::UPDATE){

         std::size_t colonPos3 = line.find(':', colonPos2 + 1);
         if(colonPos3 == std::string::npos){
            databaseLogFileIn.close();
            return ReturnCode::DATABASE_FILE_CORRUPT_ERROR;
         }         
         std::size_t colonPos4 = line.find(':', colonPos3 + 1);
         if(colonPos4 == std::string::npos){
            databaseLogFileIn.close();
            return ReturnCode::DATABASE_FILE_CORRUPT_ERROR;
         }
         
         int key = atoi(line.substr(colonPos2 + 1, colonPos3 - colonPos2 - 1).c_str());
         std::string value = line.substr(colonPos3 + 1, colonPos4 - colonPos3 - 1);
         
         this->index[key] = value;
      }
      else if(method == Method::DELETE){
         int key = std::atoi(line.substr(colonPos2 + 1).c_str());
         this->index.erase(key);
      }
      else{
         databaseLogFileIn.close();
         return ReturnCode::DATABASE_FILE_CORRUPT_ERROR;
      }
   }

   this->status = true;
   return ReturnCode::SUCCESS;
}


ReturnCode Index::readIndexEntry(const int& key, std::string* value){
   if(this->index.count(key)){
      if(value){
         *value = this->index[key];
      }
      return ReturnCode::SUCCESS;
   }
   return ReturnCode::FAILURE;
}

ReturnCode Index::writeIndexEntry(const int& key, const std::string& value){
   if(this->index.count(key)){
      return ReturnCode::KEY_ALREADY_EXIST;
   }
   else{
      this->index[key] = value;
      return ReturnCode::SUCCESS;
   }
}

ReturnCode Index::updateIndexEntry(const int& key, const std::string& value){
   if(this->index.count(key)){
      this->index[key] = value;
      return ReturnCode::SUCCESS;
   }
   else{
      return ReturnCode::KEY_NOT_FOUND;
   }
}

ReturnCode Index::deleteIndexEntry(const int& key){
   if(this->index.count(key)){
      this->index.erase(key);
      return ReturnCode::SUCCESS;
   }
   else{
      return ReturnCode::KEY_NOT_FOUND;
   }
}
