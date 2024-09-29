#pragma once

#ifndef __PROTOCOL__
#define __PROTOCOL__

#include <string>
#include <vector>

struct Query {
    std::string id;
    std::string type;
    std::string value;

    Query(const std::string& _id, const std::string& _type, const std::string& _val) :
        id(_id), type(_type), value(_val) {}
};

enum ERRCODE : uint8_t {
    SUCCESS = 0,
    ID_NOT_FOUND = 1,
    WRONG_PIN = 2,
    INVALID_ACC_INDEX = 3,
    ACCESS_DENIED = 4,
    INVALID_AMOUNT = 5,
    INSUFFICIENT_BAL = 6,
    DISCONNECTED = 7,
    UNDEFINED = 255
};

struct Response {
    std::string type;
    ERRCODE status;
    int value;

    Response() {}
    Response(const std::string& _type, const ERRCODE& _status, const int& _val) :
        type(_type), status(_status), value(_val) {}
};

#endif