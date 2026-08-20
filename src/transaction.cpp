#include <vector>
#include <string>
#include <iostream>
#include "transaction.h"


Instruction::Instruction(std::string& method, int key, int value=0) 
: method(method), key(key), value(value) {}


int Transaction::id = 0;

Transaction::Transaction(){
   this->tid = id++;
}

void Transaction::addInstruction(std::string method, int key, int value=0){
   Instruction* i = new Instruction(method, key, value);
   this->instructions.push_back(i);
}