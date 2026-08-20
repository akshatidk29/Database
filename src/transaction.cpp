#include <vector>
#include <string>
#include <iostream>

#include "database.h"
#include "transaction.h"


Instruction::Instruction(const std::string& method, const int& key, const std::string& value="", std::string* readValue=nullptr, const bool& printValue=false) 
: method(method), key(key), value(value), readValue(readValue), printValue(printValue) {}


int Transaction::id = 0;

Transaction::Transaction(){
   this->tid = id++;
}

void Transaction::addInstruction(const std::string& method, const int& key, const std::string& value="", std::string* readValue=nullptr, const bool& printValue=false){
   Instruction* i = new Instruction(method, key, value, readValue, printValue);
   this->instructions.push_back(i);
}


void Transaction::_read(const int& key, std::string* readValue, const bool& printValue=false){
   this->addInstruction("READ", key, "", readValue, printValue);
}
void Transaction::_write(const int& key, const std::string& value){
   this->addInstruction("WRITE", key, value);
}
void Transaction::_update(const int& key, const std::string& value){
   this->addInstruction("UPDATE", key, value);
}
void Transaction::_delete(const int& key){
   this->addInstruction("DELETE", key);
}



// Start Transaction
void Database::startTransaction(Transaction* txn){

   int numInstructions = txn->instructions.size();

   for(Instruction* instruction : txn->instructions){

      if(instruction->method == "WRITE"){
         this->writeEntry(instruction->key, instruction->value);
      }
      else if(instruction->method == "UPDATE"){
         this->updateEntry(instruction->key, instruction->value);
      }
      else if(instruction->method == "DELETE"){
         this->deleteEntry(instruction->key);
      }
      else if(instruction->method == "READ"){
         this->readEntry(instruction->key, instruction->readValue, instruction->printValue);
      }
   }

   std::cout << "Transaction completed!" << std::endl;
}