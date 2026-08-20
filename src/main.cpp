#include <fstream>
#include <iostream>
#include "database.h"

int main(){

   std::ofstream databaseStoreOut("data/databaseStore.db", std::ios::app);
   if(!databaseStoreOut.is_open()){
      std::ofstream databaseStoreOut("data/databaseStore.db");
   }
   databaseStoreOut.close();
   
   Database d1(101, "MyPassword", true);

   d1.writeEntry(0, "000");
   d1.writeEntry(1, "100");
   d1.writeEntry(2, "200"); 

   std::cout << d1.readEntry(1) << std::endl;
   std::cout << d1.readEntry(2) << std::endl; 

   d1.updateEntry(2, "300");
   std::cout << d1.readEntry(2) << std::endl;
   
   d1.deleteEntry(2);
   std::cout << d1.readEntry(2) << std::endl;

   return 0;
}