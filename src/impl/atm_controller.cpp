#include "header/atm_controller.hpp"


AtmController::AtmController() {}
AtmController::~AtmController() 
{
    disconnect();
}

void AtmController::connect(const std::shared_ptr<DummyBankServer>& server)
{
    _server = server;
}

void AtmController::disconnect()
{
    _server = nullptr;
}

ERRCODE AtmController::insert_card(const std::string& id)
{
    if (!_server) return ERRCODE::DISCONNECTED;
    _card_id = id;
    auto res = _server->query(Query(_card_id, "check_id", ""));
    return res.status;
}

ERRCODE AtmController::query_pin(const std::string& input_pin)
{
    if (!_server) return ERRCODE::DISCONNECTED;
    auto res = _server->query(Query(_card_id, "query_pin", input_pin));
    return res.status;
}

ERRCODE AtmController::select_account(const int& target_index)
{
    if (!_server) return ERRCODE::DISCONNECTED;
    auto res = _server->query(Query(_card_id, "sel_acc", std::to_string(target_index)));
    return res.status;
}

int AtmController::get_balance()
{
    if (!_server) return -1;
    auto res = _server->query(Query(_card_id, "get_balance", ""));
    if (res.status == ERRCODE::SUCCESS) return res.value;
    else return -1;
}

int AtmController::deposit(const int& amount)
{
    if (!_server) return -1;
    auto res = _server->query(Query(_card_id, "deposit", std::to_string(amount)));
    if (res.status == ERRCODE::SUCCESS) return res.value;
    else return -1;
}

int AtmController::withdraw(const int& amount)
{
    if (!_server) return -1;
    auto res = _server->query(Query(_card_id, "withdraw", std::to_string(amount)));
    if (res.status == ERRCODE::SUCCESS) return res.value;
    else if (res.status == ERRCODE::INSUFFICIENT_BAL) return res.value;
    else return -1;
}



