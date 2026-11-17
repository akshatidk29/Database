#include <string>
#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <unordered_map>
#include <vector>
#include <shared_mutex>
#include <mutex>

#include "core/transaction.h"
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

   ReturnCode dataCheck = readDataFile();

   if(dataCheck == ReturnCode::SUCCESS){

      ReturnCode logCheck = readLogFile();

      if(logCheck == ReturnCode::SUCCESS){
         return ReturnCode::SUCCESS;
      }

      return ReturnCode::FAILURE;
   }

   return ReturnCode::FAILURE;
}

ReturnCode Index::readDataFile(){
   
   std::string databaseDataName = std::to_string(id) + ".db";
   std::string databaseDataPath =  "data/" + std::to_string(id) + "/" + databaseDataName;

   std::ifstream databaseDataFileIn(databaseDataPath);
   if(!databaseDataFileIn.is_open()){
      return ReturnCode::DATABASE_FILE_ERROR;
   }
   
   std::string line;

   while(std::getline(databaseDataFileIn, line)){
      
      std::size_t colonPos1 = line.find(':');
      if(colonPos1 == std::string::npos){
         databaseDataFileIn.close();
         return ReturnCode::DATABASE_FILE_CORRUPT_ERROR;
      }

      std::size_t colonPos2 = line.find(':', colonPos1 + 1);
      if(colonPos2 == std::string::npos){
         databaseDataFileIn.close();
         return ReturnCode::DATABASE_FILE_CORRUPT_ERROR;
      }
      
      int key = atoi(line.substr(0, colonPos1).c_str());
      std::string value = line.substr(colonPos1 + 1, colonPos2 - colonPos1 - 1);
      
      this->index[key] = value;
   }

   databaseDataFileIn.close();
   return ReturnCode::SUCCESS;
}

ReturnCode Index::readLogFile(){
   
   std::string databaseLogName = std::to_string(id) + ".log";
   std::string databaseLogPath =  "data/" + std::to_string(id) + "/" + databaseLogName;

   std::ifstream databaseLogFileIn(databaseLogPath);
   if(!databaseLogFileIn.is_open()){
      return ReturnCode::LOG_FILE_ERROR;
   }

   struct LogOperation {
      Method method;
      int key;
      std::string value;
   };

   std::unordered_map<int, std::vector<LogOperation>> stagedTransactions;
   int currentTid = -1;
   int maxTid = -1;
   int lastLsn = 0;
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

      int lsn = std::atoi(line.substr(0, colonPos1).c_str());
      if(lsn > lastLsn){
         lastLsn = lsn;
      }

      std::string token = line.substr(colonPos1 + 1, colonPos2 - colonPos1 - 1);

      if(token == "TXN-START"){
         std::size_t colonPos3 = line.find(':', colonPos2 + 1);
         if(colonPos3 == std::string::npos){
            databaseLogFileIn.close();
            return ReturnCode::DATABASE_FILE_CORRUPT_ERROR;
         }
         int tid = std::atoi(line.substr(colonPos2 + 1, colonPos3 - colonPos2 - 1).c_str());
         if(tid > maxTid){
            maxTid = tid;
         }
         currentTid = tid;
         stagedTransactions[currentTid] = std::vector<LogOperation>();
      }else if(token == "TXN-END"){
         std::size_t colonPos3 = line.find(':', colonPos2 + 1);
         if(colonPos3 == std::string::npos){
            databaseLogFileIn.close();
            return ReturnCode::DATABASE_FILE_CORRUPT_ERROR;
         }
         int tid = std::atoi(line.substr(colonPos2 + 1, colonPos3 - colonPos2 - 1).c_str());
         if(tid > maxTid){
            maxTid = tid;
         }
         if(stagedTransactions.find(tid) != stagedTransactions.end()){
            for(const LogOperation& op : stagedTransactions[tid]){
               if(op.method == Method::WRITE || op.method == Method::UPDATE){
                  this->index[op.key] = op.value;
               }else if(op.method == Method::DELETE){
                  this->index.erase(op.key);
               }
            }
            stagedTransactions.erase(tid);
         }
         if(currentTid == tid){
            currentTid = -1;
         }
      }else if(token == "TXN-ABORT"){
         std::size_t colonPos3 = line.find(':', colonPos2 + 1);
         if(colonPos3 == std::string::npos){
            databaseLogFileIn.close();
            return ReturnCode::DATABASE_FILE_CORRUPT_ERROR;
         }
         int tid = std::atoi(line.substr(colonPos2 + 1, colonPos3 - colonPos2 - 1).c_str());
         if(tid > maxTid){
            maxTid = tid;
         }
         if(stagedTransactions.find(tid) != stagedTransactions.end()){
            stagedTransactions.erase(tid);
         }
         if(currentTid == tid){
            currentTid = -1;
         }
      }else{
         Method method = getMethod(token);

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
            
            int key = std::atoi(line.substr(colonPos2 + 1, colonPos3 - colonPos2 - 1).c_str());
            std::string value = line.substr(colonPos3 + 1, colonPos4 - colonPos3 - 1);
            
            if(currentTid != -1){
               stagedTransactions[currentTid].push_back({method, key, value});
            }else{
               this->index[key] = value;
            }
         }
         else if(method == Method::DELETE){
            std::size_t colonPos3 = line.find(':', colonPos2 + 1);
            int key = std::atoi(line.substr(colonPos2 + 1, (colonPos3 != std::string::npos) ? (colonPos3 - colonPos2 - 1) : std::string::npos).c_str());
            if(currentTid != -1){
               stagedTransactions[currentTid].push_back({method, key, ""});
            }else{
               this->index.erase(key);
            }
         }
         else{
            databaseLogFileIn.close();
            return ReturnCode::DATABASE_FILE_CORRUPT_ERROR;
         }
      }
   }

   databaseLogFileIn.close();

   if(!stagedTransactions.empty()){
      std::ofstream logFileAppend(databaseLogPath, std::ios::app);
      if(logFileAppend.is_open()){
         for(const auto& pair : stagedTransactions){
            int uncommittedTid = pair.first;
            lastLsn++;
            logFileAppend << std::to_string(lastLsn) << ":TXN-ABORT:" << std::to_string(uncommittedTid) << ":" << std::endl;
         }
         logFileAppend.close();
      }
      stagedTransactions.clear();
   }

   if(maxTid >= 0){
      Transaction::setNextTid(maxTid + 1);
   }

   this->status = true;
   return ReturnCode::SUCCESS;
}


ReturnCode Index::readIndexEntry(const int& key, std::string* value){
   std::shared_lock<std::shared_mutex> lock(this->indexMutex);
   if(this->index.count(key)){
      if(value){
         *value = this->index[key];
      }
      return ReturnCode::SUCCESS;
   }
   return ReturnCode::FAILURE;
}

ReturnCode Index::writeIndexEntry(const int& key, const std::string& value){
   std::unique_lock<std::shared_mutex> lock(this->indexMutex);
   if(this->index.count(key)){
      return ReturnCode::KEY_ALREADY_EXIST;
   }
   else{
      this->index[key] = value;
      return ReturnCode::SUCCESS;
   }
}

ReturnCode Index::updateIndexEntry(const int& key, const std::string& value){
   std::unique_lock<std::shared_mutex> lock(this->indexMutex);
   if(this->index.count(key)){
      this->index[key] = value;
      return ReturnCode::SUCCESS;
   }
   else{
      return ReturnCode::KEY_NOT_FOUND;
   }
}

ReturnCode Index::deleteIndexEntry(const int& key){
   std::unique_lock<std::shared_mutex> lock(this->indexMutex);
   if(this->index.count(key)){
      this->index.erase(key);
      return ReturnCode::SUCCESS;
   }
   else{
      return ReturnCode::KEY_NOT_FOUND;
   }
}

ReturnCode Index::getNextEntry(std::pair<int, std::string>* nextEntry){
   std::unique_lock<std::shared_mutex> lock(this->indexMutex);
   if(this->index.empty()){
      return ReturnCode::FAILURE;
   }

   static std::unordered_map<int, std::string>::iterator it = this->index.begin();

   if(it == this->index.end()){
      it = this->index.begin();
      return ReturnCode::FAILURE;
   }

   nextEntry->first = it->first;
   nextEntry->second = it->second;

   it++;

   return ReturnCode::SUCCESS;
}

ReturnCode Index::getAllEntries(std::vector<std::pair<int, std::string>>* entries){
   std::shared_lock<std::shared_mutex> lock(this->indexMutex);
   if(entries == nullptr){
      return ReturnCode::FAILURE;
   }

   for(const auto& entry : this->index){
      entries->push_back(entry);
   }

   return ReturnCode::SUCCESS;
}