#include <iostream>
#include "utils/command.h"

Command getCommand(std::string &cmd){
    if(cmd == "CREATE")     return Command::CREATE;
    if(cmd == "CONNECT")    return Command::CONNECT;
    if(cmd == "DISCONNECT") return Command::DISCONNECT;
    if(cmd == "EXIT")       return Command::EXIT;
    if(cmd == "READ")       return Command::READ;
    if(cmd == "WRITE")      return Command::WRITE;
    if(cmd == "UPDATE")     return Command::UPDATE;
    if(cmd == "DELETE")     return Command::DELETE;
    if(cmd == "BEGIN")      return Command::BEGIN;
    if(cmd == "COMMIT")     return Command::COMMIT;
    return Command::UNKNOWN;
}