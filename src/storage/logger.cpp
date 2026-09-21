#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "utils/method.h"
#include "utils/return.h"
#include "storage/logger.h"

Logger::Logger(const int& id)
   : id(id), currentLsn(1), status(false) {
   ReturnCode check = setPath();
   if(check == ReturnCode::SUCCESS){
      this->status = true;
      std::cout << "Logger Intialized!" << std::endl;
   }
}

bool Logger::getStatus(){
   return this->status;
}

ReturnCode Logger::setLSN(){

   std::string line;
   std::ifstream logfileIn(this->logFilePath);

   if(!logfileIn.is_open()){
      return ReturnCode::LOG_FILE_ERROR;
   }

   while(std::getline(logfileIn, line)){
      this->currentLsn++;
   }

   logfileIn.close();
   return ReturnCode::SUCCESS;
}

int Logger::getLSN(){
   return this->currentLsn;
}

ReturnCode Logger::setPath(){
   
   std::string fileName = std::to_string(id) + ".log"; 
   std::string logFilePath = "data/" + std::to_string(id) + "/" + fileName;
   this->logFilePath = logFilePath;

   std::ofstream logFileOut(logFilePath, std::ios::app);

   if(!logFileOut.is_open())
      return ReturnCode::FAILURE;

   logFileOut.close();

   return setLSN();
}

std::string Logger::getInstructionLog(Method method, const int& key, const std::string* value, const std::string* prevValue){
   
   std::string log;

   switch(method){

      case Method::WRITE:{
         log = std::to_string(this->currentLsn) + std::string(":") + getMethodStr(method) + std::string(":") + std::to_string(key) + std::string(":") + *value + std::string("::");
         break;
      }

      case Method::UPDATE:{
         log = std::to_string(this->currentLsn) + std::string(":") + getMethodStr(method) + std::string(":") + std::to_string(key) + std::string(":") + *value + std::string(":") + *prevValue + std::string(":");
         break;
      }

      case Method::DELETE:{
         log = std::to_string(this->currentLsn) + std::string(":") + getMethodStr(method) + std::string(":") + std::to_string(key) + std::string(":::");
         break;
      }

      default:{
         return "";
      }
   }

   this->currentLsn++;
   return log;
}

ReturnCode Logger::addTransactionLogs(std::vector<std::string> &logs){
   std::ofstream logFileOut(this->logFilePath, std::ios::app);
   if(!logFileOut.is_open())
      return ReturnCode::LOG_FILE_ERROR;
   
   for(std::string log : logs){
      logFileOut << log << std::endl;
   }

   logFileOut.close();
   return ReturnCode::SUCCESS;
}

ReturnCode Logger::clearLogFile(){
   std::ofstream logFileOut(this->logFilePath, std::ios::trunc);
   if(!logFileOut.is_open()){
      return ReturnCode::LOG_FILE_ERROR;
   }
   logFileOut.close();
   this->currentLsn = 1;
   return ReturnCode::SUCCESS;
}

