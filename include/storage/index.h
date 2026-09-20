#pragma once

#include <string>
#include <vector>
#include <unordered_map>

#include "utils/return.h"

class Index{
private:
   const int id;
   bool status;
   std::unordered_map<int, std::string> index;

   ReturnCode readLogFile();
   ReturnCode readDataFile();
   
public:
   Index(const int& id);
   bool getStatus();
   ReturnCode buildIndex();

   ReturnCode getNextEntry(std::pair<int, std::string>* nextEntry);
   ReturnCode getAllEntries(std::vector<std::pair<int, std::string>>* entries);

   ReturnCode readIndexEntry(const int& key, std::string* value);
   ReturnCode writeIndexEntry(const int& key, const std::string& value);
   ReturnCode updateIndexEntry(const int& key, const std::string& value);
   ReturnCode deleteIndexEntry(const int& key);
};
