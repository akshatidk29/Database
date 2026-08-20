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
   
   std::string password = "MyPassword";
   Database d1(101, password, true);

   Transaction t1 = Transaction();

   std::string* value = new std::string;

   t1._write(1, "100");
   t1._write(2, "200");
   t1._write(3, "300");
   t1._read(2, value, true);
   t1._update(2, "000");
   t1._read(2, value, true);
   t1._delete(2);
   t1._read(2, value, true);

   d1.startTransaction(&t1);
   
   return 0;
}