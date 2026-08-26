#include <string>
#include <fstream>
#include <iostream>
#include <unordered_map>

#include "return.h"
#include "index.h"

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

   // do some work with unordermap of index


   return ReturnCode::SUCCESS;
}


ReturnCode readIndexEntry(const int& key){

}

ReturnCode writeIndexEntry(const int& key, std::string* value){

}

ReturnCode updateIndexEntry(const int& key, std::string* value){

}
