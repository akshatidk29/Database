#pragma once

#include <vector>
#include <string>
#include <iostream>

#include "method.h"

class Instruction{
public:
   const int key;
   const bool printValue;
   const std::string value;
   std::string* readValue;

   const Method method;

   Instruction(const Method& method, const int& key, const std::string& value, std::string* readValue, const bool& printValue);
};

class Transaction{
private:
   int tid;
   static int id;

   void addInstruction(const Method& method, const int& key, const std::string& value, std::string* readValue, const bool& printValue);

public:
   std::vector<Instruction*> instructions;

   Transaction();

   void _delete(const int& key);
   void _write(const int& key, const std::string& value);
   void _update(const int& key, const std::string& value);
   void _read(const int& key, std::string* readValue, const bool& printValue);
};