#include <string>
#include <vector>
#include <iostream>

#include "interactive/shell.h"
#include "core/database.h"
#include "core/transaction.h"


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

std::string Shell::getCommand(){
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
    while(true){
        std::string command = getCommand();
        std::vector<std::string> tokens = tokenizer(command);
        if(tokens.empty()) 
            getCommand();

        for(std::string &token : tokens)
            std::cout << token << " " << std::endl;

        if(tokens[0] == "EXIT") 
            break;
    }
}