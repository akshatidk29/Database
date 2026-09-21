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

   int getLSN(); 
   bool getStatus();
   
   ReturnCode setPath();
   ReturnCode setLSN();

   std::string getInstructionLog(Method method, const int& key, const std::string* value, const std::string* prevValue);
   ReturnCode addTransactionLogs(std::vector<std::string> &logs);
   
   ReturnCode clearLogFile();
};