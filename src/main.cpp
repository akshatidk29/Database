#include <fstream>
#include <iostream>
#include "database.h"
#include "transaction.h"

int main(){

   std::ofstream databaseStoreOut("data/databaseStore.db", std::ios::app);
   if(!databaseStoreOut.is_open()){
      std::ofstream databaseStoreOut("data/databaseStore.db");
   }
   databaseStoreOut.close();
   
   Database d1(101, "MyPassword", true);

   Transaction t1 = Transaction();

   return 0;
}