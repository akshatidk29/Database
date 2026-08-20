#pragma once

#include <vector>
#include <string>
#include <iostream>

class Instruction{
public:
   int key;
   int value;
   std::string method;

   Instruction(std::string& method, int key, int value);
};

class Transaction{
private:
   int tid;
   static int id;

public:
   std::vector<Instruction*> instructions;

   Transaction();
   void addInstruction(std::string method, int key, int value = 0);
};