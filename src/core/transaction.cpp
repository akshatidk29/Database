#include <vector>
#include <string>
#include <iostream>
#include <unordered_map>
#include <unordered_set>

#include "utils/method.h"
#include "utils/return.h"
#include "storage/index.h"
#include "storage/store.h"
#include "core/database.h"
#include "core/transaction.h"


Instruction::Instruction(const Method& method, const int& key, const std::string& value="", std::string* readValue=nullptr, const bool& printValue=false) 
: method(method), key(key), value(value), readValue(readValue), printValue(printValue) {}


int Transaction::id = 0;

Transaction::Transaction(){
   this->tid = id++;
}

Transaction::~Transaction() {
   for (Instruction* instruction : instructions) {
      delete instruction;
   }
}

int Transaction::getTid(){
   return this->tid;
}

void Transaction::setNextTid(const int& nextId){
   if(nextId > id){
      id = nextId;
   }
}

void Transaction::addInstruction(const Method& method, const int& key, const std::string& value="", std::string* readValue=nullptr, const bool& printValue=false){
   Instruction* i = new Instruction(method, key, value, readValue, printValue);
   this->instructions.push_back(i);
}


void Transaction::_read(const int& key, std::string* readValue, const bool& printValue=false){
   this->addInstruction(Method::READ, key, "", readValue, printValue);
}
void Transaction::_write(const int& key, const std::string& value){
   this->addInstruction(Method::WRITE, key, value);
}
void Transaction::_update(const int& key, const std::string& value){
   this->addInstruction(Method::UPDATE, key, value);
}
void Transaction::_delete(const int& key){
   this->addInstruction(Method::DELETE, key);
}



// Start Transaction
void Database::startTransaction(Transaction* txn){

   if(txn == nullptr || !(this->access) || this->logger == nullptr){
      std::cout << "Access denied!" << std::endl;
      return;
   }

   std::string log;
   std::vector<std::string> logs;
   std::unordered_map<int, std::string> memoryIndex;
   std::unordered_set<int> deletedKeys;

   for(Instruction* instruction : txn->instructions){

      if(instruction->method == Method::WRITE){
         bool exists = false;
         if(deletedKeys.count(instruction->key)){
            exists = false;
         }else if(memoryIndex.count(instruction->key)){
            exists = true;
         }else{
            std::string dummy;
            ReturnCode idxCheck = this->index->readIndexEntry(instruction->key, &dummy);
            if(idxCheck == ReturnCode::SUCCESS){
               exists = true;
            }
         }

         if(exists){
            std::cout << "Key already exists!" << std::endl; 
            this->logger->addAbortLog(txn->getTid());
            std::cout << "Transaction aborted!" << std::endl;
            return;
         }else{
            log = this->logger->getInstructionLog(instruction->method, instruction->key, &(instruction->value), nullptr);
            if(log == ""){
               std::cout << "Internal server error!" << std::endl;
               this->logger->addAbortLog(txn->getTid());
               std::cout << "Transaction aborted!" << std::endl;
               return;
            }else{
               logs.push_back(log);
               memoryIndex[instruction->key] = instruction->value;
               deletedKeys.erase(instruction->key);
            }
         }
      }

      else if(instruction->method == Method::UPDATE){

         std::string previousValue;
         bool exists = false;

         if(deletedKeys.count(instruction->key)){
            exists = false;
         }else if(memoryIndex.count(instruction->key)){
            exists = true;
            previousValue = memoryIndex[instruction->key];
         }else{
            ReturnCode idxCheck = this->index->readIndexEntry(instruction->key, &previousValue);
            if(idxCheck == ReturnCode::SUCCESS){
               exists = true;
            }
         }

         if(!exists){
            std::cout << "Key not found!" << std::endl; 
            this->logger->addAbortLog(txn->getTid());
            std::cout << "Transaction aborted!" << std::endl;
            return;
         }else{
            log = this->logger->getInstructionLog(instruction->method, instruction->key, &(instruction->value), &previousValue);
            if(log == ""){
               std::cout << "Internal server error!" << std::endl;
               this->logger->addAbortLog(txn->getTid());
               std::cout << "Transaction aborted!" << std::endl;
               return;
            }else{
               logs.push_back(log);
               memoryIndex[instruction->key] = instruction->value;
            }
         }
      }

      else if(instruction->method == Method::DELETE){

         bool exists = false;

         if(deletedKeys.count(instruction->key)){
            exists = false;
         }else if(memoryIndex.count(instruction->key)){
            exists = true;
         }else{
            std::string dummy;
            ReturnCode idxCheck = this->index->readIndexEntry(instruction->key, &dummy);
            if(idxCheck == ReturnCode::SUCCESS){
               exists = true;
            }
         }

         if(!exists){
            std::cout << "Key not found!" << std::endl; 
            this->logger->addAbortLog(txn->getTid());
            std::cout << "Transaction aborted!" << std::endl;
            return;
         }else{
            log = this->logger->getInstructionLog(instruction->method, instruction->key, nullptr, nullptr);
            if(log == ""){
               std::cout << "Internal server error!" << std::endl;
               this->logger->addAbortLog(txn->getTid());
               std::cout << "Transaction aborted!" << std::endl;
               return;
            }else{
               logs.push_back(log);
               memoryIndex.erase(instruction->key);
               deletedKeys.insert(instruction->key);
            }
         }
      }
   }

   ReturnCode logCheck = this->logger->addTransactionLogs(txn->getTid(), logs);

   if(logCheck == ReturnCode::SUCCESS){

      for(Instruction* instruction : txn->instructions){

         if(instruction->method == Method::WRITE){
            this->writeEntry(instruction->key, instruction->value);
         }else if(instruction->method == Method::UPDATE){
            this->updateEntry(instruction->key, instruction->value);
         }else if(instruction->method == Method::DELETE){
            this->deleteEntry(instruction->key);
         }else if(instruction->method == Method::READ){
            this->readEntry(instruction->key, instruction->readValue, instruction->printValue);
         }
      }

      std::cout << "Transaction completed!" << std::endl;

   }else{
      std::cout << "Transaction failed!" << std::endl;
   }
}