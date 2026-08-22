#pragma once
#include <string>
#include <iostream>

#include "return.h"
#include "method.h"

class Logger{
private:
   int id;
   bool status;
   int currentLsn;
   std::string logFilePath;
   
public:
   Logger(const int& id);
   bool getStatus();
   ReturnCode setLSN();
   ReturnCode setPath();
   ReturnCode addLog(Method method, const int& key, const std::string* value, const std::string* prevValue);
};