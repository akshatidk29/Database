#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <vector>

#include "utils/return.h"
#include "utils/method.h"
#include "storage/store.h"

ReturnCode checkDatabaseExistence(const int& id){
   
   std::ifstream databaseStoreIn("data/databaseStore.db");
   if(!databaseStoreIn.is_open()){
      return ReturnCode::FAILURE;
   }

   std::string line;

   while (std::getline(databaseStoreIn, line))
   {
      std::size_t colonPos = line.find(':');
      if (colonPos == std::string::npos)
      {  
         databaseStoreIn.close();
         return ReturnCode::DATABASE_STORE_CORRUPT_ERROR;
      }

      std::string presentId = line.substr(0, colonPos);
      if (presentId == std::to_string(id))
      {
         databaseStoreIn.close();
         return ReturnCode::SUCCESS;
      }
   }

   databaseStoreIn.close();
   return ReturnCode::FAILURE;
}

ReturnCode addNewDatabase(const int& id, const std::string& password){

   std::error_code directoryError;
   std::filesystem::create_directories("data", directoryError);
   if (directoryError)
   {
      return ReturnCode::DIRECTORY_ERROR;
   }

   ReturnCode check = checkDatabaseExistence(id);

   if (check == ReturnCode::SUCCESS)
   {
      return ReturnCode::FAILURE;
   }

   if(check != ReturnCode::FAILURE){
      return check;
   }

   std::string databaseDirectory = "data/" + std::to_string(id);
   std::filesystem::create_directories(databaseDirectory, directoryError);
   if (directoryError)
   {
      return ReturnCode::DIRECTORY_ERROR;
   }

   std::string databaseFilePath = databaseDirectory + "/" + std::to_string(id) + ".db";
   std::ofstream databaseFileOut(databaseFilePath, std::ios::app);

   if (!databaseFileOut.is_open())
   {
      return ReturnCode::DATABASE_FILE_ERROR;
   }

   databaseFileOut.close();
   
   std::ofstream databaseStoreOut("data/databaseStore.db", std::ios::app);

   if (!databaseStoreOut.is_open())
   {
      return ReturnCode::DATABASE_STORE_ERROR;
   }

   databaseStoreOut << id << ":" << password << std::endl;
   databaseStoreOut.close();

   return ReturnCode::SUCCESS;
}

ReturnCode authorizeDatabaseAccess(const int& id, const std::string& password){

   std::ifstream databaseStoreIn("data/databaseStore.db");
   std::string line;

   if(!databaseStoreIn.is_open()){
      return ReturnCode::FAILURE;
   }

   while (std::getline(databaseStoreIn, line))
   {

      std::size_t colonPos = line.find(':');
      if (colonPos == std::string::npos)
      {
         databaseStoreIn.close();
         return ReturnCode::DATABASE_STORE_CORRUPT_ERROR;
      }

      std::string presentId = line.substr(0, colonPos);
      std::string presentPassword = line.substr(colonPos + 1);

      if (presentId == std::to_string(id) && presentPassword == password)
      {
         databaseStoreIn.close();
         return ReturnCode::SUCCESS;
      }
   }

   databaseStoreIn.close();
   return ReturnCode::FAILURE;
}


ReturnCode changeDatabasePassword(const int& id, const std::string& previousPassword, const std::string& newPassword){

   std::ifstream databaseStoreIn("data/databaseStore.db");

   if(!databaseStoreIn.is_open()){
      return ReturnCode::DATABASE_STORE_ERROR;
   }

   std::string line;
   std::vector<std::string> buffer;

   bool found = false;

   while (std::getline(databaseStoreIn, line))
   {
      std::size_t colonPos = line.find(':');
      if (colonPos == std::string::npos)
      {
         databaseStoreIn.close();
         return ReturnCode::DATABASE_STORE_CORRUPT_ERROR;
      }

      std::string presentId = line.substr(0, colonPos);
      std::string presentPassword = line.substr(colonPos + 1);

      if (presentId == std::to_string(id) && presentPassword == previousPassword)
      {
         found = true;
      }else{
         buffer.push_back(line);
      }
   }
   databaseStoreIn.close();

   if(found == true){

      std::ofstream databaseStoreOut("data/databaseStore.db");
      
      if(!databaseStoreOut.is_open()){
         return ReturnCode::DATABASE_STORE_ERROR;
      }
      for(std::string& line : buffer){
         databaseStoreOut << line << std::endl;
      }
      
      databaseStoreOut << id << ":" << newPassword << std::endl;

      databaseStoreOut.close();

      return ReturnCode::SUCCESS;
   }

   return ReturnCode::WROND_CREDENTIALS;
}

ReturnCode writeDatabaseEntry(const int& id, const int& key, const std::string& value){

   std::string databaseFilePath = "data/" + std::to_string(id) + "/" + std::to_string(id) + ".db";
   std::ofstream databaseFileOut(databaseFilePath, std::ios::app);

   if(!databaseFileOut.is_open()){
      return ReturnCode::DATABASE_FILE_ERROR;
   }

   databaseFileOut << key << ':' << Method::WRITE << ':' << value << std::endl;
   databaseFileOut.close();

   return ReturnCode::SUCCESS;
}

ReturnCode updateDatabaseEntry(const int& id, const int& key, const std::string& value){

   std::string databaseFilePath = "data/" + std::to_string(id) + "/" + std::to_string(id) + ".db";
   std::ofstream databaseFileOut(databaseFilePath, std::ios::app);

   if(!databaseFileOut.is_open()){
      return ReturnCode::DATABASE_FILE_ERROR;
   }

   databaseFileOut << key << ':' << Method::UPDATE << ':' << value << std::endl;
   databaseFileOut.close();

   return ReturnCode::SUCCESS;
}

ReturnCode deleteDatabaseEntry(const int& id, const int& key){
  
   std::string databaseFilePath = "data/" + std::to_string(id) + "/" + std::to_string(id) + ".db";
   std::ofstream databaseFileOut(databaseFilePath, std::ios::app);

   if(!databaseFileOut.is_open()){
      return ReturnCode::DATABASE_FILE_ERROR;
   }

   databaseFileOut << key << ':' << Method::DELETE << ':' << std::endl;
   databaseFileOut.close();

   return ReturnCode::SUCCESS;
}