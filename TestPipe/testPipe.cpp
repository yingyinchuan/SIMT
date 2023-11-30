#include <iostream>
#include <cstdio>
#include <string>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#define N 2

using namespace std;

void writer(int wfd)
{
    const char *s = "hello,i'm child";
    pid_t self = getpid();
    int num = 1;

    char buffer[1024];

    while (true)
    {
        buffer[0] = 0;
        snprintf(buffer, sizeof(buffer), "%s-%d-%d", s, self, num++);
        write(wfd, buffer, strlen(buffer));
        sleep(1);
    }
}
void reader(int rfd)
{
    char buffer[1024] = {0};
    while (true)
    {
        buffer[0] = 0;
        ssize_t n = read(rfd, buffer, sizeof(buffer));
        if (n > 0)
        {
            buffer[n] = 0;
            cout << "father get a message[" << getpid() << "]# " << buffer << endl;
        }
        else if (n == 0)
        {
            printf("father read file done!\n");
            break;
        }
        else
            break;
    }
}

main()
{
    int pipefd[N] = {0};
    int n = pipe(pipefd); // 创建管道
    if (n < 0)
        return 1;

    pid_t id = fork();
    if (id < 0)
        return 1;

    if (id == 0)
    {
        close(pipefd[0]);
        writer(pipefd[1]);
        exit(0);
    }
    close(pipefd[1]);
    reader(pipefd[0]);
    pid_t pid = waitpid(-1, nullptr, 0);

    if (pid < 0)
        return 3;
    return 0;
}