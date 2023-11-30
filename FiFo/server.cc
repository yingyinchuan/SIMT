#include "comm.hpp"

using namespace std;

int main()
{
    int fd = open(FIFO_FILE, O_RDONLY);
    if (fd < 0)
    {
        perror("open");
        exit(FIFO_OPEN_ERR);
    }

    char buffer[1024];
    while (true)
    {
        int n = read(fd, buffer, sizeof(buffer));
        if (n != 0)
        {
            buffer[n] = 0;
            cout << "get massage:" << buffer << endl;
        }
        else if (n == 0)
        {
            cout << "client is quit,me too" << endl;
            break;
        }
        else
            break;
    }

    close(fd);

    return 0;
}