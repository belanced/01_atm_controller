#include "header/atm_controller.hpp"
#include <gtest/gtest.h>

class AtmControllerTest: public testing::Test {
protected:
    std::shared_ptr<DummyBankServer> server;
    std::shared_ptr<AtmController> atm;

    void SetUp() override
    {
        server = std::make_shared<DummyBankServer>();
        if (!server) std::cerr << "Failed to initialize server!" << std::endl;
        server->set_account("#001", "1000", {100, 200});
        server->set_account("#002", "1004", {500});
        server->set_account("#004", "1234", {200, 100, 0});

        atm = std::make_shared<AtmController>();
        atm->connect(server);        
    }

    void TearDown() override
    {
        server = nullptr;
        atm = nullptr;
    }
};

TEST_F(AtmControllerTest, TestCardInsert)
{
    EXPECT_EQ(atm->insert_card("#001"), ERRCODE::SUCCESS);
    EXPECT_EQ(atm->insert_card("#002"), ERRCODE::SUCCESS);
    EXPECT_EQ(atm->insert_card("#003"), ERRCODE::ID_NOT_FOUND);
    EXPECT_EQ(atm->insert_card("#002"), ERRCODE::SUCCESS);
}

TEST_F(AtmControllerTest, TestQueryPin)
{
    EXPECT_EQ(atm->insert_card("#001"), ERRCODE::SUCCESS);
    EXPECT_EQ(atm->query_pin("0000"), ERRCODE::WRONG_PIN);
    EXPECT_EQ(atm->query_pin("1000"), ERRCODE::SUCCESS);

    EXPECT_EQ(atm->insert_card("#002"), ERRCODE::SUCCESS);
    EXPECT_EQ(atm->query_pin("1004"), ERRCODE::SUCCESS);
    EXPECT_EQ(atm->query_pin("1234"), ERRCODE::WRONG_PIN);

    EXPECT_EQ(atm->insert_card("#003"), ERRCODE::ID_NOT_FOUND);
    EXPECT_EQ(atm->query_pin("1234"), ERRCODE::ID_NOT_FOUND);

    EXPECT_EQ(atm->insert_card("#004"), ERRCODE::SUCCESS);
    EXPECT_EQ(atm->query_pin("1234"), ERRCODE::SUCCESS);
}

TEST_F(AtmControllerTest, TestSelectAccount)
{
    EXPECT_EQ(atm->insert_card("#001"), ERRCODE::SUCCESS);
    EXPECT_EQ(atm->query_pin("1000"), ERRCODE::SUCCESS);
    EXPECT_EQ(atm->select_account(-1), ERRCODE::INVALID_ACC_INDEX);
    EXPECT_EQ(atm->select_account(0), ERRCODE::SUCCESS);
    EXPECT_EQ(atm->select_account(1), ERRCODE::SUCCESS);
    EXPECT_EQ(atm->select_account(2), ERRCODE::INVALID_ACC_INDEX);

    EXPECT_EQ(atm->insert_card("#002"), ERRCODE::SUCCESS);
    EXPECT_EQ(atm->query_pin("1000"), ERRCODE::WRONG_PIN);
    EXPECT_EQ(atm->select_account(0), ERRCODE::ACCESS_DENIED);
    EXPECT_EQ(atm->query_pin("1004"), ERRCODE::SUCCESS);
    EXPECT_EQ(atm->select_account(-1), ERRCODE::INVALID_ACC_INDEX);
    EXPECT_EQ(atm->select_account(0), ERRCODE::SUCCESS);
    EXPECT_EQ(atm->select_account(1), ERRCODE::INVALID_ACC_INDEX);
    EXPECT_EQ(atm->select_account(2), ERRCODE::INVALID_ACC_INDEX);

    EXPECT_EQ(atm->insert_card("#003"), ERRCODE::ID_NOT_FOUND);
    EXPECT_EQ(atm->query_pin("1000"), ERRCODE::ID_NOT_FOUND);
    EXPECT_EQ(atm->select_account(-1), ERRCODE::ID_NOT_FOUND);
    EXPECT_EQ(atm->select_account(0), ERRCODE::ID_NOT_FOUND);
    EXPECT_EQ(atm->select_account(1), ERRCODE::ID_NOT_FOUND);
    EXPECT_EQ(atm->select_account(2), ERRCODE::ID_NOT_FOUND);

    EXPECT_EQ(atm->insert_card("#004"), ERRCODE::SUCCESS);
    EXPECT_EQ(atm->query_pin("1000"), ERRCODE::WRONG_PIN);
    EXPECT_EQ(atm->select_account(0), ERRCODE::ACCESS_DENIED);
    EXPECT_EQ(atm->query_pin("1234"), ERRCODE::SUCCESS);
    EXPECT_EQ(atm->select_account(-1), ERRCODE::INVALID_ACC_INDEX);
    EXPECT_EQ(atm->select_account(0), ERRCODE::SUCCESS);
    EXPECT_EQ(atm->select_account(1), ERRCODE::SUCCESS);
    EXPECT_EQ(atm->select_account(2), ERRCODE::SUCCESS);
}

TEST_F(AtmControllerTest, TestCheckBalance)
{
    EXPECT_EQ(atm->insert_card("#001"), ERRCODE::SUCCESS);
    EXPECT_EQ(atm->query_pin("1001"), ERRCODE::WRONG_PIN);
    EXPECT_EQ(atm->select_account(0), ERRCODE::ACCESS_DENIED);
    EXPECT_EQ(atm->query_pin("1000"), ERRCODE::SUCCESS);
    EXPECT_EQ(atm->select_account(0), ERRCODE::SUCCESS);
    EXPECT_EQ(atm->get_balance(), 100);
    EXPECT_EQ(atm->select_account(1), ERRCODE::SUCCESS);
    EXPECT_EQ(atm->get_balance(), 200);

    EXPECT_EQ(atm->insert_card("#002"), ERRCODE::SUCCESS);
    EXPECT_EQ(atm->query_pin("1004"), ERRCODE::SUCCESS);
    EXPECT_EQ(atm->select_account(0), ERRCODE::SUCCESS);
    EXPECT_EQ(atm->get_balance(), 500);
    EXPECT_EQ(atm->select_account(1), ERRCODE::INVALID_ACC_INDEX); 
    // invalid index does not update selected_index
    // thus, it returns the balace of index 0
    EXPECT_EQ(atm->get_balance(), 500); 

    EXPECT_EQ(atm->insert_card("#003"), ERRCODE::ID_NOT_FOUND);
    EXPECT_EQ(atm->query_pin("1000"), ERRCODE::ID_NOT_FOUND);
    EXPECT_EQ(atm->select_account(0), ERRCODE::ID_NOT_FOUND);
    EXPECT_EQ(atm->get_balance(), -1);

    EXPECT_EQ(atm->insert_card("#004"), ERRCODE::SUCCESS);
    EXPECT_EQ(atm->query_pin("1234"), ERRCODE::SUCCESS);
    EXPECT_EQ(atm->select_account(-1), ERRCODE::INVALID_ACC_INDEX);
    EXPECT_EQ(atm->select_account(0), ERRCODE::SUCCESS);
    EXPECT_EQ(atm->select_account(1), ERRCODE::SUCCESS);
    EXPECT_EQ(atm->select_account(2), ERRCODE::SUCCESS);
    EXPECT_EQ(atm->get_balance(), 0);
    EXPECT_EQ(atm->select_account(3), ERRCODE::INVALID_ACC_INDEX);
    EXPECT_EQ(atm->get_balance(), 0);
}

TEST_F(AtmControllerTest, TestDepositWithdraw)
{
    EXPECT_EQ(atm->insert_card("#001"), ERRCODE::SUCCESS);
    EXPECT_EQ(atm->query_pin("1000"), ERRCODE::SUCCESS);
    EXPECT_EQ(atm->select_account(0), ERRCODE::SUCCESS);
    EXPECT_EQ(atm->deposit(100), 200);
    EXPECT_EQ(atm->get_balance(), 200);
    EXPECT_EQ(atm->select_account(1), ERRCODE::SUCCESS);
    EXPECT_EQ(atm->deposit(50), 250);
    EXPECT_EQ(atm->get_balance(), 250);
    EXPECT_EQ(atm->withdraw(100), 150);
    EXPECT_EQ(atm->withdraw(150), 0);
    // insufficient balance
    EXPECT_EQ(atm->withdraw(100), 0);

    EXPECT_EQ(atm->insert_card("#002"), ERRCODE::SUCCESS);
    EXPECT_EQ(atm->query_pin("1004"), ERRCODE::SUCCESS);
    EXPECT_EQ(atm->select_account(0), ERRCODE::SUCCESS);
    EXPECT_EQ(atm->deposit(100), 600);
    EXPECT_EQ(atm->get_balance(), 600);
    EXPECT_EQ(atm->select_account(1), ERRCODE::INVALID_ACC_INDEX);
    EXPECT_EQ(atm->withdraw(50), 550);
    EXPECT_EQ(atm->get_balance(), 550);

    // for id that does not exist
    EXPECT_EQ(atm->insert_card("#003"), ERRCODE::ID_NOT_FOUND);
    EXPECT_EQ(atm->query_pin("1000"), ERRCODE::ID_NOT_FOUND);
    EXPECT_EQ(atm->select_account(0), ERRCODE::ID_NOT_FOUND);
    EXPECT_EQ(atm->get_balance(), -1);
    EXPECT_EQ(atm->deposit(100), -1);
    EXPECT_EQ(atm->withdraw(100), -1);

    EXPECT_EQ(atm->insert_card("#004"), ERRCODE::SUCCESS);
    // wrong pin
    EXPECT_EQ(atm->query_pin("1004"), ERRCODE::WRONG_PIN);
    EXPECT_EQ(atm->select_account(0), ERRCODE::ACCESS_DENIED);
    EXPECT_EQ(atm->deposit(100), -1);
    EXPECT_EQ(atm->get_balance(), -1);
    EXPECT_EQ(atm->query_pin("1234"), ERRCODE::SUCCESS);
    // not selected acc index
    EXPECT_EQ(atm->deposit(100), -1); 
    // select account 0
    EXPECT_EQ(atm->select_account(0), ERRCODE::SUCCESS);
    EXPECT_EQ(atm->deposit(100), 300);
    EXPECT_EQ(atm->select_account(2), ERRCODE::SUCCESS);
    EXPECT_EQ(atm->deposit(100), 100);
    EXPECT_EQ(atm->select_account(1), ERRCODE::SUCCESS);
    EXPECT_EQ(atm->deposit(100), 200);
}

int main(int argc, char** argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}