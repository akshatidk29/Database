#pragma once

#include <string>
#include <vector>
#include <iostream>
#include "core/database.h"
#include "core/transaction.h"


class Shell{
private:
    Database *db;
    Transaction *txn;
    std::vector<std::string> tokenizer(std::string);
    std::string getCommand();

public:
    void start();
};