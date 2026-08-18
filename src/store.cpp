#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "store.h"
#include "return.h"

int numDatabases = 0;

ReturnCode checkDatabaseExistence(int id)
{
   std::ifstream databaseStoreIn("data/databaseStore.db");
   if(!databaseStoreIn.is_open()){
      return ReturnCode::DATABASE_STORE_ERROR;
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

ReturnCode addNewDatabase(int id, std::string password)
{
   ReturnCode check = checkDatabaseExistence(id);

   if (check == ReturnCode::SUCCESS)
   {
      return ReturnCode::FAILURE;
   }

   if(check != ReturnCode::FAILURE){
      return check;
   }

   std::string databasePath = "data/" + std::to_string(id) + ".db";
   std::ofstream databaseFileOut(databasePath, std::ios::app);

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


   numDatabases++;

   return ReturnCode::SUCCESS;
}


ReturnCode authorizeDatabaseAccess(int id, std::string password)
{

   std::ifstream databaseStoreIn("data/databaseStore.db");
   std::string line;

   if(!databaseStoreIn.is_open()){
      return ReturnCode::DATABASE_STORE_ERROR;
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

ReturnCode changeDatabasePassword(int id, std::string previousPassword, std::string newPassword)
{

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


ReturnCode readDatabase(int id, int key, std::string& value){
   
   ReturnCode check = checkDatabaseExistence(id);
   
   if(check == ReturnCode::FAILURE){
      return ReturnCode::DATABASE_NOT_FOUND;
   }
   else if(check != ReturnCode::SUCCESS){
      return check;
   }

   std::string databasePath = "data/" + std::to_string(id) + ".db";
   std::ifstream databaseFileIn(databasePath);

   if(!databaseFileIn.is_open()){
      return ReturnCode::DATABASE_FILE_ERROR;
   }

   std::string line;
   
   bool found = false;

   while(std::getline(databaseFileIn, line)){
      
      std::size_t colonPos = line.find(':');

      if(colonPos == std::string::npos){
         databaseFileIn.close();
         return ReturnCode::DATABASE_FILE_CORRUPT_ERROR;
      }
      std::string presentKey = line.substr(0, colonPos);
      if(presentKey == std::to_string(key)){
         found = true;
         value = line.substr(colonPos + 1);
      }
   }
   databaseFileIn.close();

   if(found){
      return ReturnCode::SUCCESS;
   }
   return ReturnCode::FAILURE;
}

ReturnCode writeDatabase(int id, int key, std::string value){

   std::string temporary;
   ReturnCode check = readDatabase(id, key, temporary);
   
   if(check == ReturnCode::SUCCESS){
      return ReturnCode::KEY_ALREADY_EXIST;
   }
   if(check != ReturnCode::FAILURE){
      return check;
   }

   std::string databasePath = "data/" + std::to_string(id) + ".db";
   std::ofstream databaseFileOut(databasePath, std::ios::app);

   if(!databaseFileOut.is_open()){
      return ReturnCode::DATABASE_FILE_ERROR;
   }

   databaseFileOut << key << ":" << value << std::endl;
   databaseFileOut.close();

   return ReturnCode::SUCCESS;
}

ReturnCode updateDatabase(int id, int key, std::string value){

   ReturnCode check = checkDatabaseExistence(id);
   
   if(check == ReturnCode::FAILURE){
      return ReturnCode::DATABASE_NOT_FOUND;
   }
   else if(check != ReturnCode::SUCCESS){
      return check;
   }

   std::string temporary;
   ReturnCode keyCheck = readDatabase(id, key, temporary);

   if (keyCheck == ReturnCode::FAILURE)
   {
      return ReturnCode::KEY_NOT_FOUND;
   }

   if (keyCheck != ReturnCode::SUCCESS)
   {
      return keyCheck;
   }

   std::string databasePath = "data/" + std::to_string(id) + ".db";
   std::ofstream databaseFileOut(databasePath, std::ios::app);

   if(!databaseFileOut.is_open()){
      return ReturnCode::DATABASE_FILE_ERROR;
   }

   databaseFileOut << key << ":" << value << std::endl;

   return ReturnCode::SUCCESS;
}