#include <fstream>
#include <iostream>
#include <filesystem>
#include "core/database.h"
#include "core/transaction.h"
#include "interactive/shell.h"

int main(){

   std::filesystem::create_directories("data");
   std::ofstream databaseStoreOut("data/databaseStore.db", std::ios::app);
   databaseStoreOut.close();
   
   Shell s1 = Shell();
   s1.start();
   
   return 0;
}