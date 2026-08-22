#include <iostream>
#include <fstream>
#include <string>

#include "logger.h"
#include "method.h"

Logger::Logger(const int& id)
   : id(id)
{
   setPath();
   std::cout << "Logger Intialized!" << std::endl;
}

void Logger::setPath(){
   std::string fileName = std::to_string(id) + ".log"; 
   std::string logFilePath = "data/" + std::to_string(id) + "/" + fileName;
   this->logFilePath = logFilePath;

   std::ofstream logFileOut(logFilePath, std::ios::app);
   logFileOut.close();
}

void Logger::addLog(std::string method, const int& key, std::string* Value, std::string* prevValue){
   return;
}