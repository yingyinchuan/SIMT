#include <functional>
#include <string>
#include <signal.h>
#include "Socket.hpp"

using func_t = std::function<std::string(const std::string &package)>;

class TcpServer
{
public:
    TcpServer(uint16_t port, func_t callback) : port_(port), callback_(callback) {}

    bool InitServer()
    {
        try
        {
            listensock_->Socket();
            listensock_->Bind(port_);
            listensock_->Listen();
            return true;
        }
        catch (const std::exception &e)
        {
            return false;
        }
    }

    void Start()
    {
        signal(SIGCHLD, SIG_IGN);
        signal(SIGPIPE, SIG_IGN);
        while (true)
        {
            std::string clientip;
            uint16_t clientport;
            int sockfd = listensock_->Accept(clientip, clientport);
            if (sockfd < 0)
                continue;
            if (fork() == 0)
            {
                listensock_->Close();
                std::string inbuffer_stream;
                // 数据计算
                while (true)
                {
                    char buffer[1280];
                    ssize_t n = read(sockfd, buffer, sizeof(buffer));
                    if (n > 0)
                    {
                        buffer[n] = 0;
                        inbuffer_stream += buffer;

                        while (true)
                        {
                            std::string info = callback_(inbuffer_stream);
                            if (info.empty())
                                break;
                            write(sockfd, info.c_str(), info.size());
                        }
                    }
                    else if (n == 0)
                        break;
                    else
                        break;
                }

                exit(0);
            }
            close(sockfd);
        }
    }

    ~TcpServer() {}

private:
    uint16_t port_;
    Sock *listensock_;
    func_t callback_;
};
