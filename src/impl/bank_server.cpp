#include "header/bank_server.hpp"

DummyBankServer::DummyBankServer() {}
DummyBankServer::~DummyBankServer() {}

bool DummyBankServer::set_account(const std::string& id, const std::string& pin, const std::vector<int>& balances)
{
    /*
        Account setter for DummyBankServer. Should be replaced later when real implementation.
    */
    _db.emplace(id, Account(pin, balances));
    return true;
}

Response DummyBankServer::query(const Query& query)
{
    auto it = _db.find(query.id);
    if (it == _db.end())
        return Response(query.type, ERRCODE::ID_NOT_FOUND, 0);

    if (query.type == "check_id") 
        return Response(query.type, ERRCODE::SUCCESS, 0); 

    if (query.type == "query_pin")   
    {
        if (query.value == it->second.pin) 
        {
            it->second.is_authorized = true;
            return Response(query.type, ERRCODE::SUCCESS, 0);
        } 
        else 
        {
            it->second.is_authorized = false;
            return Response(query.type, ERRCODE::WRONG_PIN, 0);
        }
    }

    if (!it->second.is_authorized)  
        return Response(query.type, ERRCODE::ACCESS_DENIED, 0);

    if (query.type == "close") 
    {
        it->second.is_authorized = false;
        it->second.selected_acc = -1;
        return Response(query.type, ERRCODE::SUCCESS, 0);
    }

    if (query.type == "sel_acc") 
    {
        try   
        {
            int target_index = std::stoi(query.value);
            if (target_index >= 0 && target_index < it->second.balances.size()) 
            {
                it->second.selected_acc = target_index;
                return Response("sel_acc", ERRCODE::SUCCESS, 0);
            } 
            else 
            {
                return Response("sel_acc", ERRCODE::INVALID_ACC_INDEX, 0);
            }
        } 
        catch (const std::exception&) 
        {
            return Response("sel_acc", ERRCODE::INVALID_ACC_INDEX, 0);
        }
    }

    if (it->second.selected_acc < 0 || it->second.selected_acc >= it->second.balances.size())
        return Response(query.type, ERRCODE::INVALID_ACC_INDEX, 0);

    int acc_index = it->second.selected_acc;
    
    if (query.type == "get_balance") 
        return Response(query.type, ERRCODE::SUCCESS, it->second.balances[acc_index]);
    else if (query.type == "deposit") 
    {
        try 
        {
            int amount = std::stoi(query.value);
            if (amount <= 0) return Response(query.type, ERRCODE::INVALID_AMOUNT, 0);
            it->second.balances[acc_index] += amount;
            return Response(query.type, ERRCODE::SUCCESS, it->second.balances[acc_index]);
        } 
        catch (const std::exception&) 
        {
            return Response(query.type, ERRCODE::INVALID_AMOUNT, 0);
        }
    } 
    else if (query.type == "withdraw") 
    {
        try 
        {
            int amount = std::stoi(query.value);
            if (amount <= 0) return Response(query.type, ERRCODE::INVALID_AMOUNT, 0);
            if (amount > it->second.balances[acc_index]) 
                return Response(query.type, ERRCODE::INSUFFICIENT_BAL, it->second.balances[acc_index]);
            it->second.balances[acc_index] -= amount;
            return Response(query.type, ERRCODE::SUCCESS, it->second.balances[acc_index]);
        } 
        catch (const std::exception&) 
        {
            return Response(query.type, ERRCODE::INVALID_AMOUNT, 0);
        }
    }

    return Response(query.type, ERRCODE::UNDEFINED, 0);
}


