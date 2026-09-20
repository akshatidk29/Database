#pragma once

#include <string>
#include <iostream>

#include "utils/return.h"
#include "utils/method.h"

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
   int getLSN();
   ReturnCode setPath();
   ReturnCode addLog(Method method, const int& key, const std::string* value, const std::string* prevValue);
   
   ReturnCode clearLogFile();
};