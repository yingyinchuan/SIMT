#pragma once

#include <iostream>
#include <string>
#include <sstream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

const std::string BLANK_SPACE_SEP = " ";
const std::string PROTOCOL_SEP = "\n";

std::string Encode(const std::string &content)
{
    std::stringstream package;
    package << content.size() << PROTOCOL_SEP << content << PROTOCOL_SEP;
    return package.str();
}

bool Decode(std::string &package, std::string *content)
{
    std::size_t pos = package.find(PROTOCOL_SEP);
    if (pos == std::string::npos)
        return false;

    std::string len_str = package.substr(0, pos);
    std::size_t len = std::stoi(len_str);
    std::size_t total_len = len_str.size() + len + 2;

    if (package.size() < total_len)
        return false;

    *content = package.substr(pos + 1, len);
    package.erase(0, total_len);

    return true;
}

class Request
{
public:
    Request(int data1, int data2, char oper) : x(data1), y(data2), op(oper) {}

    bool Serialize(std::string *out) const
    {
        json data = {
            {"x", x},
            {"y", y},
            {"op", op}};
        *out = data.dump();
        return true;
    }

    bool Deserialize(const std::string &in)
    {
        json data = json::parse(in);
        x = data["x"];
        y = data["y"];
        op = data["op"];
        return true;
    }

    void DebugPrint() const
    {
        std::cout << "New request: " << x << op << y << "=?" << std::endl;
    }

private:
    int x;
    int y;
    char op;
};

class Response
{
public:
    Response(int res, int c) : result(res), code(c) {}

    bool Serialize(std::string *out) const
    {
        json data = {
            {"result", result},
            {"code", code}};
        *out = data.dump();
        return true;
    }

    bool Deserialize(const std::string &in)
    {
        json data = json::parse(in);
        result = data["result"];
        code = data["code"];
        return true;
    }

    void DebugPrint() const
    {
        std::cout << "Response: result = " << result << ", code = " << code << std::endl;
    }

private:
    int result;
    int code;
};
