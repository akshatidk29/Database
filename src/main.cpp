#include<iostream>
#include"database.h"
#include"store.h"

int main(){
   Database d1(1);
   d1.write(0, 100);
   return 0;
}