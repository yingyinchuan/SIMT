#pragma once

#include <iostream>
#include <string>
#include <unistd.h>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

const int SocketErr = 2;
const int BindErr = 3;
const int ListenErr = 4;

class Sock
{
public:
    Sock() : sockfd_(-1) {}
    ~Sock()
    {
        if (sockfd_ != -1)
            close(sockfd_);
    }

public:
    void Socket()
    {
        sockfd_ = socket(AF_INET, SOCK_STREAM, 0);
        if (sockfd_ == -1)
            exit(SocketErr);
    }

    void Bind(uint16_t port)
    {
        struct sockaddr_in local;
        memset(&local, 0, sizeof(local));
        local.sin_family = AF_INET;
        local.sin_port = htons(port);
        local.sin_addr.s_addr = INADDR_ANY;

        if (bind(sockfd_, reinterpret_cast<struct sockaddr *>(&local), sizeof(local)) == -1)
            exit(BindErr);
    }

    void Listen()
    {
        if (listen(sockfd_, backlog) == -1)
            exit(ListenErr);
    }

    int Accept(std::string &client_ip, uint16_t &client_port)
    {
        struct sockaddr_in peer;
        socklen_t len = sizeof(peer);
        int newfd = accept(sockfd_, reinterpret_cast<struct sockaddr *>(&peer), &len);
        if (newfd == -1)
        {
            return -1;
        }
        char ipstr[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &peer.sin_addr, ipstr, sizeof(ipstr));
        client_ip = ipstr;
        client_port = ntohs(peer.sin_port);

        return newfd;
    }

    bool Connect(const std::string &ip, uint16_t port)
    {
        struct sockaddr_in peer;
        memset(&peer, 0, sizeof(peer));
        peer.sin_family = AF_INET;
        peer.sin_port = htons(port);
        if (inet_pton(AF_INET, ip.c_str(), &peer.sin_addr) != 1)
        {
            return false;
        }

        if (connect(sockfd_, reinterpret_cast<struct sockaddr *>(&peer), sizeof(peer)) == -1)
        {
            return false;
        }

        return true;
    }

    void Close()
    {
        if (sockfd_ != -1)
            close(sockfd_);
    }

    int Fd() const { return sockfd_; }

private:
    int sockfd_;
    static const int backlog = 10;
};