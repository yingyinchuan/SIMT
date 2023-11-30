#pragma once

#include <iostream>
#include <string>
#include <cstdlib>
#include <cstring>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

using namespace std;

const int size = 4096;
const string pathname = "/home/yunxuena";
const int proj_id = 0x6666;

key_t GetKey()
{
    key_t k = ftok(pathname.c_str(), proj_id);
    if (k < 0)
        exit(1);

    return k;
}

int GetSharememHelper(int flag)
{
    key_t k = GetKey();
    int shmid = shmget(k, size, flag);
    if (shmid == -1)
    {
        perror("shmget");
        exit(2);
    }

    return shmid;
}

int CreateShm()
{
    return GetSharememHelper(IPC_CREAT | IPC_EXCL | 0666);
}

int GetShm()
{
    return GetSharememHelper(IPC_CREAT);
}

#define FIFO_FILE "./myfifo"
#define MODE 0664

enum
{
    FIFO_CREATE_ERR = 1,
    FIFO_DELETE_ERR,
    FIFO_OPEN_ERR
};

class Init
{
public:
    Init()
    {
        int n = mkfifo(FIFO_FILE, MODE);
        if (n == -1)
        {
            perror("create");
            exit(FIFO_CREATE_ERR);
        }
    }
    ~Init()
    {
        int m = unlink(FIFO_FILE);
        if (m == -1)
        {
            perror("unlink");
            exit(FIFO_DELETE_ERR);
        }
    }
};