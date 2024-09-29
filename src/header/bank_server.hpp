#pragma once

#ifndef __BANK_SERVER__
#define __BANK_SERVER__

#include "protocol.hpp"

#include <iostream>
#include <vector>
#include <memory>
#include <map>

struct Account {
    bool is_authorized = false;
    int selected_acc = -1;
    std::string pin = ""; // personal PIN
    std::vector<int> balances = {}; // belonging accounts and balances

    Account() {}
    Account(const std::string& _pin, const std::vector<int>& _bals) :
        is_authorized(false), selected_acc(-1), pin(_pin), balances(_bals) {}
        
};

class DummyBankServer {
/*
    DummyBankServer that holds account database.
    This is not the real implementation. Should be replaced later.
*/
private:
    std::map<std::string, Account> _db; // <id, Account> table
    
public:
    DummyBankServer();
    ~DummyBankServer();

    bool set_account(const std::string& id, const std::string& pin, const std::vector<int>& balances);

    Response query(const Query& query);
};

#endif