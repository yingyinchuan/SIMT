#include "comm.hpp"

using namespace std;

int main()
{
    int shmid = GetShm();
    char *shmddr = (char *)shmat(shmid, nullptr, 0);

    int fd = open(FIFO_FILE, O_WRONLY);
    if (fd < 0)
    {
        perror("open");
        exit(FIFO_OPEN_ERR);
    }

    while (true)
    {
        cout << "Please Enter@";
        if (!fgets(shmddr, 4096, stdin))
            break;
        write(fd, "c", 1);
    }

    shmdt(shmddr);

    close(fd);

    return 0;
}