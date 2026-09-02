#include <iostream>
#include <fstream>
#include <string>

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

ReturnCode Logger::addLog(Method method, const int& key, const std::string* value, const std::string* prevValue){
   
   std::ofstream logFileOut(this->logFilePath, std::ios::app);
   if(!logFileOut.is_open()){
      return ReturnCode::LOG_FILE_ERROR;
   }

   switch(method){

      case Method::WRITE:{
         logFileOut << this->currentLsn << ':' << Method::WRITE << ':' << key << ':' << *value << ':' << std::endl;
         break;
      }

      case Method::UPDATE:{
         logFileOut << this->currentLsn << ':' << Method::UPDATE << ':' << key << ':' << *value << ':' << *prevValue << std::endl;
         break;
      }

      case Method::DELETE:{
         logFileOut << this->currentLsn << ':' << Method::DELETE << ':' << key  << "::" << std::endl;
         break;
      }

      default:{
         return ReturnCode::FAILURE;
      }
   }

   logFileOut.close();
   this->currentLsn++;
   return ReturnCode::SUCCESS;
}