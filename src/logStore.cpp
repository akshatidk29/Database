#include <iostream>

#include "store.h"
#include "return.h"


ReturnCode checkKeyExistence(const int& id, const int& key){
   
   ReturnCode check = readDatabaseEntry(id, key, nullptr);
   
   if(check == ReturnCode::SUCCESS){
      return ReturnCode::KEY_ALREADY_EXIST;
   }
   if(check == ReturnCode::FAILURE){
      return ReturnCode::KEY_NOT_FOUND;
   }
   return ReturnCode::FAILURE;
}