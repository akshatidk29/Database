#pragma once

#include <iostream>

enum class Command{
   CREATE,
   CONNECT,
   DISCONNECT,
   BEGIN,
   COMMIT,
   READ,
   WRITE,
   UPDATE,
   DELETE,
   EXIT,
   UNKNOWN,
};

Command getCommand(std::string &cmd);