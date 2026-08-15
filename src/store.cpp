#include <iostream>
#include <fstream>
#include "store.h"
#include <string>
#include <vector>

int numDatabases = 0;

bool checkDatabase(int id)
{

   std::ifstream databaseStore("../data/databaseStore.db");
   std::string line;

   while (std::getline(databaseStore, line))
   {

      int colonPos = line.find(':');
      if (colonPos == std::string::npos)
      {
         continue;
      }

      std::string presentId = line.substr(0, colonPos);
      if (presentId == std::to_string(id))
      {
         databaseStore.close();
         return true;
      }
   }

   databaseStore.close();
   return false;
}

bool authorizeDatabase(int id, int password)
{

   std::ifstream databaseStore("../data/databaseStore.db");
   std::string line;

   while (std::getline(databaseStore, line))
   {

      int colonPos = line.find(':');
      if (colonPos == std::string::npos)
      {
         continue;
      }

      std::string presentId = line.substr(0, colonPos);
      std::string presentPassword = line.substr(colonPos);

      if (presentId == std::to_string(id) && presentPassword == std::to_string(password))
      {
         databaseStore.close();
         return true;
      }
   }

   databaseStore.close();
   return false;
}

bool changePassword(int id, int previousPassword, int newPassword)
{

   std::ifstream databaseStore("../data/databaseStore.db");
   std::string line;
   std::vector<std::string> buffer;

   bool found = false;

   while (std::getline(databaseStore, line))
   {
      int colonPos = line.find(':');
      if (colonPos == std::string::npos)
      {
         continue;
      }
      std::string presentId = line.substr(0, colonPos);
      std::string presentPassword = line.substr(colonPos);

      if (presentId == std::to_string(id) && presentPassword == std::to_string(previousPassword))
      {
         found = true;
      }else{
         buffer.push_back(line);
      }
   }
   databaseStore.close();

   std::ofstream databaseStore2("../data/databaseStore.db");

   for(std::string& line : buffer){
      databaseStore2 << line << std::endl;
   }

   if(found){
      databaseStore2 << id << ":" << newPassword << std::endl;
      std::cout << "Password Changed!\n";
      return true;
   }

   return false;
}


int addDatabase(int id, int password)
{

   bool check = checkDatabase(id);

   if (check)
   {
      return 1;
   }

   std::ofstream databaseStore("../data/databaseStore.db");

   if (!databaseStore.is_open())
   {
      return 2;
   }

   databaseStore << id << ":" << password << std::endl;
   databaseStore.close();

   std::string databasePath = "../data/" + std::to_string(id) + ".db";
   std::ofstream databaseFile(databasePath);

   if (!databaseFile.is_open())
   {
      return 3;
   }

   databaseFile.close();

   numDatabases++;

   return id;
}