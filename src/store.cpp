#include <iostream>
#include <fstream>
#include "store.h"
#include <string>
#include <vector>

int numDatabases = 0;

bool checkDatabaseExistence(int id)
{

   std::ifstream databaseStoreIn("../data/databaseStore.db");
   std::string line;

   while (std::getline(databaseStoreIn, line))
   {
      int colonPos = line.find(':');
      if (colonPos == std::string::npos)
      {
         continue;
      }

      std::string presentId = line.substr(0, colonPos);
      if (presentId == std::to_string(id))
      {
         databaseStoreIn.close();
         return true;
      }
   }

   databaseStoreIn.close();
   return false;
}

bool addNewDatabase(int id, std::string password)
{

   bool check = checkDatabaseExistence(id);

   if (check)
   {
      std::cout << "Database with same ID existed already!" << std::endl;
      return false;
   }

   std::ofstream databaseStoreOut("../data/databaseStore.db");

   if (!databaseStoreOut.is_open())
   {
      std::cout << "Error Occured while opening Database Store." << std::endl;
      return false;
   }

   databaseStoreOut << id << ":" << password << std::endl;
   databaseStoreOut.close();

   std::string databasePath = "../data/" + std::to_string(id) + ".db";
   std::ofstream databaseFileOut(databasePath);

   if (!databaseFileOut.is_open())
   {
      std::cout << "Error Occured while opening Database File" << std::endl;
      return false;
   }

   databaseFileOut.close();

   numDatabases++;

   return true;
}

bool authorizeDatabaseAccess(int id, std::string password)
{

   std::ifstream databaseStoreIn("../data/databaseStore.db");
   std::string line;

   while (std::getline(databaseStoreIn, line))
   {

      int colonPos = line.find(':');
      if (colonPos == std::string::npos)
      {
         continue;
      }

      std::string presentId = line.substr(0, colonPos);
      std::string presentPassword = line.substr(colonPos);

      if (presentId == std::to_string(id) && presentPassword == password)
      {
         databaseStoreIn.close();
         return true;
      }
   }

   databaseStoreIn.close();
   return false;
}

bool changeDatabasePassword(int id, std::string previousPassword, std::string newPassword)
{

   std::ifstream databaseStoreIn("../data/databaseStore.db");
   std::string line;
   std::vector<std::string> buffer;

   bool found = false;

   while (std::getline(databaseStoreIn, line))
   {
      int colonPos = line.find(':');
      if (colonPos == std::string::npos)
      {
         continue;
      }

      std::string presentId = line.substr(0, colonPos);
      std::string presentPassword = line.substr(colonPos);

      if (presentId == std::to_string(id) && presentPassword == previousPassword)
      {
         found = true;
      }else{
         buffer.push_back(line);
      }
   }
   databaseStoreIn.close();

   std::ofstream databaseStoreOut("../data/databaseStore.db");

   for(std::string& line : buffer){
      databaseStoreOut << line << std::endl;
   }

   if(found){
      databaseStoreOut << id << ":" << newPassword << std::endl;
      std::cout << "Password Changed!\n";
      return true;
   }

   return false;
}

