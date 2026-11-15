#include <string>
#include <vector>
#include <iostream>

#include "interactive/shell.h"
#include "core/database.h"
#include "core/transaction.h"
#include "utils/command.h"

std::vector<std::string> Shell::tokenizer(std::string command){
    std::vector<std::string> tokens;
    std::string currentToken;
    bool inString = false;

    for(char c : command){
        if(c == '"'){
            inString = !inString;
            if(!currentToken.empty()){
                tokens.push_back(currentToken);
                currentToken.clear();
            }
        }else if(c == ' '){
            if(!inString){
                if(!currentToken.empty()){
                    tokens.push_back(currentToken);
                    currentToken.clear();
                }
            }
        }else{
            currentToken += c;
        }
    }

    if(!currentToken.empty()){
        tokens.push_back(currentToken);
    }

    for(char &c : tokens[0]){
        if(c >= 'a' && c <= 'z')
            c = c - 32;
    }

    return tokens;
}

std::string Shell::getCommandLine(){
    std::string p;
    
    if(!this->db) 
        p = "db> ";
    else
        p = "db[" + std::to_string(this->db->getId()) + "]> ";

    std::string command;
    std::cout << p;
    std::getline(std::cin, command);
    return command;
}

void Shell::start(){

    std::string *dummy = new std::string;

    while(true){
        std::string commandLine = getCommandLine();
        std::vector<std::string> tokens = tokenizer(commandLine);
        if(tokens.empty()) 
            getCommandLine();

        Command cmd = getCommand(tokens[0]);

        switch(cmd){
            case Command::CREATE:{
                if(this->db)
                    std::cout << "You are already connected to a database" << std::endl;
                else if(tokens.size() != 3)
                    std::cout << "Usage: CREATE <DATABASE-NAME> <Password>" << std::endl;
                else
                    this->db = new Database(atoi(tokens[1].c_str()), tokens[2], true);
                break;
            }
            case Command::CONNECT:{
                if(this->db)
                    std::cout << "You are already connected to a database" << std::endl;
                else if(tokens.size() != 3)
                    std::cout << "Usage: CONNECT <DATABASE-ID> <Password>" << std::endl;
                else
                    this->db = new Database(atoi(tokens[1].c_str()), tokens[2], false);
                break;
            }
            case Command::DISCONNECT:{
                if(!this->db)
                    std::cout << "You are not connected to any database" << std::endl;
                else{
                    delete this->db;
                    this->db = nullptr;
                }
                break;
            }
            case Command::READ:{
                if(!this->db)
                    std::cout << "You are not connected to any database" << std::endl;
                if(txn){
                    if(tokens.size() != 2)
                        std::cout << "Usage: READ <key>" << std::endl;
                    else
                        this->txn->_read(atoi(tokens[1].c_str()), dummy, true);
                }else{
                    this->txn = new Transaction();
                    this->txn->_read(atoi(tokens[1].c_str()), dummy, true);
                    this->db->startTransaction(this->txn);
                    delete this->txn;
                    this->txn = nullptr;
                }
                break;
            }
            case Command::WRITE:{
                if(!this->db)
                    std::cout << "You are not connected to any database" << std::endl;
                if(this->txn){
                    if(tokens.size() != 3)
                        std::cout << "Usage: WRITE <key> <value>" << std::endl;
                    else
                        this->txn->_write(atoi(tokens[1].c_str()), tokens[2]);
                }else{
                    this->txn = new Transaction();
                    this->txn->_write(atoi(tokens[1].c_str()), tokens[2]);
                    this->db->startTransaction(this->txn);
                    delete this->txn;
                    this->txn = nullptr;
                }
                break;
            }
            case Command::UPDATE:{
                if(!this->db)
                    std::cout << "You are not connected to any database" << std::endl;
                if(this->txn){
                    if(tokens.size() != 3)
                        std::cout << "Usage: UPDATE <key> <value>" << std::endl;
                    else
                        this->txn->_update(atoi(tokens[1].c_str()), tokens[2]);
                }else{
                    this->txn = new Transaction();
                    this->txn->_update(atoi(tokens[1].c_str()), tokens[2]);
                    this->db->startTransaction(this->txn);
                    delete this->txn;
                    this->txn = nullptr;
                }
                break;
            }
            case Command::DELETE:{
                if(!this->db)
                    std::cout << "You are not connected to any database" << std::endl;
                if(this->txn){
                    if(tokens.size() != 2)
                        std::cout << "Usage: DELETE <key>" << std::endl;
                    else
                        this->txn->_delete(atoi(tokens[1].c_str()));
                }else{
                    this->txn = new Transaction();
                    this->txn->_delete(atoi(tokens[1].c_str()));
                    this->db->startTransaction(this->txn);
                    delete this->txn;
                    this->txn = nullptr;
                }
                break;
            }
            case Command::BEGIN:{
                if(!this->db)
                    std::cout << "You are not connected to any database" << std::endl;
                else{
                    this->txn = new Transaction();
                }
                break;
            }
            case Command::COMMIT:{
                if(!this->db)
                    std::cout << "You are not connected to any database" << std::endl;
                else if(!this->txn)
                    std::cout << "You have not started any transaction! Create one using BEGIN." << std::endl;
                else{
                    this->db->startTransaction(this->txn);
                    delete this->txn;
                    this->txn = nullptr;
                }
                break;
            }
            case Command::EXIT:{
                if(this->db)
                    delete this->db;
                if(this->txn)
                    delete this->txn;
                std::cout << "BYE!" << std::endl;
                return;
            }
            default: {
                std::cout << "Invalid command." << std::endl;
            }
        }
    }
}