#pragma once

#include <fstream>
#include <iostream>

enum class Method{
   ERR,
   READ,
   WRITE,
   UPDATE,
   DELETE,
};


Method getMethod(const std::string& str);
std::string getMethodStr(const Method& method);
std::ostream& operator<<(std::ostream& os, Method method);