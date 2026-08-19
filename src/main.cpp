#include <iostream>
#include "database.h"

int main(){
   Database d1(1);
   d1.write(0, "100");
   d1.setPassword("MyPassword");
   d1.write(1, "100");
   d1.write(2, "200");
   std::cout << d1.read(1) << std::endl;
   std::cout << d1.read(2) << std::endl;

   d1.update(2, "300");
   std::cout << d1.read(2) << std::endl;
   return 0;
}