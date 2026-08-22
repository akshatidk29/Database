#pragma once
#include <string>
#include <iostream>

class Logger{
private:
   int id;
   int currentLsn;
   std::string logFilePath;
   
public:
   Logger(const int& id);
   void setPath();
   void addLog(std::string method, const int& key, std::string* Value, std::string* prevValue);
};