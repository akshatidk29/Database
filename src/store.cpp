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


ReturnCode readDatabaseEntry(int id, int key, std::string* value){
   
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
   
   bool exists = false;

   while(std::getline(databaseFileIn, line)){
      
      std::size_t colonPos1 = line.find(':');
      if(colonPos1 == std::string::npos){
         databaseFileIn.close();
         return ReturnCode::DATABASE_FILE_CORRUPT_ERROR;
      }

      std::size_t colonPos2 = line.find(':', colonPos1 + 1);
      if(colonPos2 == std::string::npos){
         databaseFileIn.close();
         return ReturnCode::DATABASE_FILE_CORRUPT_ERROR;
      }

      std::string presentKey = line.substr(0, colonPos1);
      std::string method = line.substr(colonPos1 + 1, colonPos2 - colonPos1 - 1);

      if(method == "WRITE" || method == "UPDATE"){
         if(presentKey == std::to_string(key)){
            exists = true;
            if(value)
               *value = line.substr(colonPos2 + 1);
         }
      }else if(method == "DELETE"){
         if(presentKey == std::to_string(key)){
            exists = false;
            if(value)
               *value = "";
         }
      }else{
         databaseFileIn.close();
         return ReturnCode::DATABASE_FILE_CORRUPT_ERROR;
      }
   }

   databaseFileIn.close();

   if(exists){
      return ReturnCode::SUCCESS;
   }
   return ReturnCode::FAILURE;
}

ReturnCode writeDatabaseEntry(int id, int key, const std::string& value){

   ReturnCode check = readDatabaseEntry(id, key, nullptr);
   
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

   databaseFileOut << key << ":WRITE:" << value << std::endl;
   databaseFileOut.close();

   return ReturnCode::SUCCESS;
}

ReturnCode updateDatabaseEntry(int id, int key, const std::string& value){

   ReturnCode check = readDatabaseEntry(id, key, nullptr);
   
   if(check == ReturnCode::FAILURE){
      return ReturnCode::KEY_NOT_FOUND;
   }
   if(check != ReturnCode::SUCCESS){
      return check;
   }

   std::string databasePath = "data/" + std::to_string(id) + ".db";
   std::ofstream databaseFileOut(databasePath, std::ios::app);

   if(!databaseFileOut.is_open()){
      return ReturnCode::DATABASE_FILE_ERROR;
   }

   databaseFileOut << key << ":UPDATE:" << value << std::endl;
   databaseFileOut.close();

   return ReturnCode::SUCCESS;
}

ReturnCode deleteDatabaseEntry(int id, int key){
  
   ReturnCode check = readDatabaseEntry(id, key, nullptr);
   
   if(check == ReturnCode::FAILURE){
      return ReturnCode::KEY_NOT_FOUND;
   }
   if(check != ReturnCode::SUCCESS){
      return check;
   }

   std::string databasePath = "data/" + std::to_string(id) + ".db";
   std::ofstream databaseFileOut(databasePath, std::ios::app);

   if(!databaseFileOut.is_open()){
      return ReturnCode::DATABASE_FILE_ERROR;
   }

   databaseFileOut << key << ":DELETE:" << std::endl;
   databaseFileOut.close();

   return ReturnCode::SUCCESS;
}