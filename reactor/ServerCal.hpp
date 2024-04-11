#pragma once

#include <iostream>
#include "Protocol.hpp"

constexpr int Div_Zero = 1;
constexpr int Mod_Zero = 2;
constexpr int Other_Oper = 3;

class ServerCal
{
public:
    Response CalculatorHelper(const Request &req)
    {
        Response resp(0, 0);
        switch (req.op)
        {
        case '+':
            resp.result = req.x + req.y;
            break;
        case '-':
            resp.result = req.x - req.y;
            break;
        case '*':
            resp.result = req.x * req.y;
            break;
        case '/':
            if (req.y == 0)
                resp.code = Div_Zero;
            else
                resp.result = req.x / req.y;
            break;
        case '%':
            if (req.y == 0)
                resp.code = Mod_Zero;
            else
                resp.result = req.x % req.y;
            break;
        default:
            resp.code = Other_Oper;
            break;
        }
        return resp;
    }

    std::string Calculator(const std::string &package)
    {
        std::string content;
        if (!Decode(package, &content))
            throw std::runtime_error("Failed to decode package");

        Request req;
        if (!req.Deserialize(content))
            throw std::runtime_error("Failed to deserialize request");

        Response resp = CalculatorHelper(req);

        resp.Serialize(&content);
        return Encode(content);
    }
};
