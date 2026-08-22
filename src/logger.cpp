#include <iostream>
#include <fstream>
#include <string>

#include "logger.h"
#include "method.h"
#include "return.h"
#include "logStore.h"

Logger::Logger(const int& id)
   : id(id), currentLsn(1), status(false)
{
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

   ReturnCode check = checkKeyExistence(this->id, key);

   switch(method){

      case Method::WRITE:{
         if(check == ReturnCode::KEY_NOT_FOUND){
            logFileOut << this->currentLsn << ':' << Method::WRITE << ':' << key << ':' << *value << std::endl;
         }else if(check != ReturnCode::FAILURE){
            return ReturnCode::KEY_ALREADY_EXIST;
         }else{
            return check;
         }
         break;
      }

      case Method::UPDATE:{
         if(check == ReturnCode::KEY_ALREADY_EXIST){
            logFileOut << this->currentLsn << ':' << Method::UPDATE << ':' << key << ':' << *prevValue << ':' << *value << std::endl;
         }else if(check != ReturnCode::FAILURE){
            return ReturnCode::KEY_NOT_FOUND;
         }else{
            return check;
         }
         break;
      }

      case Method::DELETE:{
         if(check == ReturnCode::KEY_ALREADY_EXIST){
            logFileOut << this->currentLsn << ':' << Method::DELETE << ':' << key << std::endl;
         }else if(check != ReturnCode::FAILURE){
            return ReturnCode::KEY_NOT_FOUND;
         }else{
            return check;
         }
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