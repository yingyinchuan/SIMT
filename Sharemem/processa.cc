#include "comm.hpp"

int main()
{
    Init init;
    int shmid = CreateShm();

    int fd = open(FIFO_FILE, O_RDONLY);
    char *shmddr = (char *)shmat(shmid, nullptr, 0);

    if (fd < 0)
    {
        perror("open");
        exit(FIFO_OPEN_ERR);
    }

    struct shmid_ds shmds;

    while (true)
    {
        char c;
        int n = read(fd, &c, 1);
        if (n == 0)
            break;
        else if (n < 0)
            break;
        shmctl(shmid, IPC_STAT, &shmds);
        cout << "client say@" << shmddr << endl;

        cout << "sharemem size:" << shmds.shm_segsz << endl;
        cout << "sharemem nattch:" << shmds.shm_nattch << endl;
        cout << "sharemem key:" << shmds.shm_perm.__key << endl;
        cout << "sharemem mode:" << shmds.shm_perm.mode << endl;
    }

    shmdt(shmddr);

    shmctl(shmid, IPC_RMID, nullptr);

    close(fd);

    return 0;
}