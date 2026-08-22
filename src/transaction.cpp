#include <vector>
#include <string>
#include <iostream>

#include "return.h"
#include "method.h"
#include "store.h"
#include "database.h"
#include "transaction.h"


Instruction::Instruction(const Method& method, const int& key, const std::string& value="", std::string* readValue=nullptr, const bool& printValue=false) 
: method(method), key(key), value(value), readValue(readValue), printValue(printValue) {}


int Transaction::id = 0;

Transaction::Transaction(){
   this->tid = id++;
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

   for(Instruction* instruction : txn->instructions){

      if(instruction->method == Method::WRITE){

         ReturnCode check = this->logger->addLog(instruction->method, instruction->key, &(instruction->value), nullptr);
         if(check == ReturnCode::KEY_ALREADY_EXIST){
               std::cout << "Key already exists!" << std::endl; 
            }else if(check != ReturnCode::SUCCESS){
            std::cout << "Internal server error!" << std::endl;
         }

         this->writeEntry(instruction->key, instruction->value);
      }

      else if(instruction->method == Method::UPDATE){

         std::string previousValue;
         ReturnCode previousCheck = readDatabaseEntry(this->id, instruction->key, &previousValue);
         ReturnCode check = previousCheck == ReturnCode::SUCCESS
            ? this->logger->addLog(instruction->method, instruction->key, &(instruction->value), &previousValue)
            : previousCheck;
         if(check == ReturnCode::KEY_NOT_FOUND){
               std::cout << "Key not found!" << std::endl; 
            }else if(check != ReturnCode::SUCCESS){
            std::cout << "Internal server error!" << std::endl;
         }

         this->updateEntry(instruction->key, instruction->value);
      }

      else if(instruction->method == Method::DELETE){

         ReturnCode check = this->logger->addLog(instruction->method, instruction->key, nullptr, nullptr);
         if(check == ReturnCode::KEY_NOT_FOUND){
               std::cout << "Key not found!" << std::endl; 
            }else if(check != ReturnCode::SUCCESS){
            std::cout << "Internal server error!" << std::endl;
         }
         
         this->deleteEntry(instruction->key);
      }

      else if(instruction->method == Method::READ){

         this->readEntry(instruction->key, instruction->readValue, instruction->printValue);
      }
   }

   std::cout << "Transaction completed!" << std::endl;
}