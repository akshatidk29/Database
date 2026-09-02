#pragma once

#include <string>
#include <unordered_map>

#include "utils/return.h"

class Index{
private:
   const int id;
   bool status;
   std::unordered_map<int, std::string> index;

public:
   Index(const int& id);
   bool getStatus();
   ReturnCode buildIndex();


   ReturnCode readIndexEntry(const int& key, std::string* value);
   ReturnCode writeIndexEntry(const int& key, const std::string& value);
   ReturnCode updateIndexEntry(const int& key, const std::string& value);
   ReturnCode deleteIndexEntry(const int& key);
};
