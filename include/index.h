#pragma once

#include <string>
#include <unordered_map>

class Index{
private:
   const int id;
   bool status;
   std::unordered_map<int, std::string> index;

public:
   Index(const int& id);
   bool getStatus();
   ReturnCode buildIndex();


   ReturnCode readIndexEntry(const int& key);
   ReturnCode writeIndexEntry(const int& key, std::string* value);
   ReturnCode updateIndexEntry(const int& key, std::string* value);
};
