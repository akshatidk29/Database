#include <fstream>
#include <iostream>
#include "method.h"

std::ostream& operator<<(std::ostream& os, Method method){
   switch(method){
      case Method::READ: 
         return os << "READ";
      case Method::WRITE:
         return os << "WRITE";      
      case Method::UPDATE:
         return os << "UPDATE";      
      case Method::DELETE:
         return os << "DELETE";
   }
   return os;
}

Method getMethod(const std::string& str) {
   if (str == "READ")   
      return Method::READ;
   if (str == "WRITE")  
      return Method::WRITE;
   if (str == "UPDATE") 
      return Method::UPDATE;
   if (str == "DELETE") 
      return Method::DELETE;
   return Method::ERR;
}