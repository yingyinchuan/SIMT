#include <iostream>
#include <unistd.h>
#include <vector>
#include "LuckGuard.hpp"

using namespace std;

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
int TicketNum = 0;

void *getTicket(void *arg)
{
    LockGuard L = &lock;
    int n = 1000;
    while (n--)
    {
        TicketNum++;
        usleep(1000);
    }
    pthread_exit(0);
}

int main()
{
    vector<pthread_t> tids;
    for (int i = 0; i < 10; i++)
    {
        pthread_t tid;
        pthread_create(&tid, nullptr, &getTicket, nullptr);
        tids.push_back(tid);
    }
    int num = 1000;
    for (auto x : tids)
        pthread_join(x, nullptr);
    cout << "抢了" << TicketNum << "张票" << endl;
    return 0;
}