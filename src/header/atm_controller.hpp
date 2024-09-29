#pragma once

#ifndef __ATM_CONTROLLER__
#define __ATM_CONTROLLER__

#include "bank_server.hpp"

#include <iostream>
#include <memory>
#include <utility>

class AtmController {
private:
    std::shared_ptr<DummyBankServer> _server;
    std::string _card_id = "";

public:
    AtmController();
    ~AtmController();

    void connect(const std::shared_ptr<DummyBankServer>& server);
    void disconnect();

    // step 1
    ERRCODE insert_card(const std::string& id);
    // step 2
    ERRCODE query_pin(const std::string& input_pin);
    // step 3
    ERRCODE select_account(const int& target_index);
    // step 4
    int get_balance();
    int deposit(const int& amount);
    int withdraw(const int& amount);
};


#endif 