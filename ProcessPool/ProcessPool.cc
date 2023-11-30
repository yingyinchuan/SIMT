#include "Task.hpp"
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <cassert>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/wait.h>

const int processnum = 10;
std::vector<task_t> tasks;

// 先描述
class channel
{
public:
    channel(int cmdfd, pid_t slaverid, std::string processname)
        : _cmdfd(cmdfd), _slaverid(slaverid), _processname(processname)
    {
    }
    int _cmdfd;
    pid_t _slaverid;
    std::string _processname;
};

void slaver()
{
    while (true)
    {
        int cmdcode = 0;
        int n = read(0, &cmdcode, sizeof(int));
        if (n == sizeof(int))
        {
            std::cout << "slaver say@ get a command:" << getpid() << ": cmdcode:" << cmdcode << std::endl;
            if (cmdcode >= 0 && cmdcode < tasks.size())
                tasks[cmdcode]();
        }
        if (n == 0)
            break;
    }
}

void InitProcessPool(std::vector<channel> *channels)
{
    for (int i = 0; i < processnum; i++)
    {
        int pipefd[2];
        int n = pipe(pipefd);
        assert(!n);

        pid_t id = fork();
        if (id == 0)
        {
            close(pipefd[1]);
            dup2(pipefd[0], 0);
            close(pipefd[0]);
            slaver();
            std::cout << "process:" << getpid() << " quit" << std::endl;
            exit(0);
        }
        close(pipefd[0]);
        std::string name = "process-" + std::to_string(i);
        channels->push_back(channel(pipefd[1], id, name));
    }
}

void Debug(const std::vector<channel> &channels)
{
    for (const auto &c : channels)
        std::cout << c._cmdfd << " " << c._processname << " " << c._slaverid << std::endl;
}

void Menu()
{
    std::cout << "################################################" << std::endl;
    std::cout << "# 1. 刷新日志             2. 刷新野怪            " << std::endl;
    std::cout << "# 3. 检测更新             4. 更新血量            " << std::endl;
    std::cout << "# 0. 退出                                       " << std::endl;
    std::cout << "################################################" << std::endl;
}

void ctrlSlaver(const std::vector<channel> &channels)
{
    int which = 0;
    while (true)
    {
        int select = 0;
        Menu();
        std::cout << "Plese Enter@";
        std::cin >> select;
        if (select <= 0 || select >= 5)
            break;
        int cmdcode = select - 1;
        std::cout << "father say: "
                  << " cmdcode: " << cmdcode
                  << " already sendto " << channels[which]._slaverid << " process name: "
                  << channels[which]._processname << std::endl;
        write(channels[which]._cmdfd, &select, sizeof(int));

        which++;
        which %= channels.size();
    }
}

void Quitprocess(const std::vector<channel> &channels)
{
    for (const auto &c : channels)
        close(c._cmdfd);
    for (const auto &c : channels)
        waitpid(c._slaverid, nullptr, 0);
}

main()
{
    LoadTask(&tasks);
    // 再组织
    std::vector<channel> channels;

    InitProcessPool(&channels);

    ctrlSlaver(channels);

    Quitprocess(channels);

    return 0;
}