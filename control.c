#include "semaphone.h"

int main(int argc, char **argv)
{
    int semd;
    int shmd;
    int fd;
    if (argc == 1) {
        printf("please provide one argument when running the program\n");
        return 0;
    }

    if (!strcmp(argv[1], "-c") && cre(semd, shmd, fd))
    {
        return cre(semd, shmd, fd);
    }

    if (!strcmp(argv[1], "-r") && rem(semd, shmd, fd))
    {   
        return rem(semd, shmd, fd);
    }
    if (!strcmp(argv[1], "-v"))
    {
        printf("The story so far:\n");
        struct stat fstat;
        stat(FILENAME, &fstat);
        fd = open(FILENAME, O_RDONLY);
        char content[fstat.st_size];
        read(fd, content, fstat.st_size);
        write(STDOUT_FILENO, content, fstat.st_size);
        close(fd);
        execlp("chmod", "chmod", "0644", FILENAME, NULL);
    }
}

int cre(int semd, int shmd, int fd) {
    semd = semget(SEMKEY, 1, IPC_CREAT | IPC_EXCL | 0644);
    if (semd < 0)
    {
        printf("error - cannot create semaphore: %s\n", strerror(errno));
        return errno;
    }
    else
    {
        union semun us;
        us.val = 1;
        semctl(semd, 0, SETVAL, us);
        printf("semaphore created\n");
    }
    //shared memory
    shmd = shmget(MEMKEY, SEG_SIZE, IPC_CREAT | IPC_EXCL | 0644);
    if (shmd < 0)
    {
        printf("error - cannot create shared memory: %s\n", strerror(errno));
        return errno;
    }
    printf("shared memory created\n");

    //file
    fd = open(FILENAME, O_CREAT | O_EXCL | 0644);
    if (fd < 0)
    {
        printf("error creating file: %s\n", strerror(errno));
        if (open(FILENAME, O_RDWR))
        {
            printf("file already created\n");
            printf("recreating file\n");
            fd = open(FILENAME, O_CREAT | 0644);
        }
        else
        {
            return errno;
        }
    }
    printf("file created\n");
    close(fd);
    return 0;
}

int rem(int semd, int shmd, int fd) {
    semd = semget(SEMKEY, 1, 0644);
    if (semd < 0)
    {
        printf("error - semaphore access denied: %s\n", strerror(errno));
        return errno;
    }
    else
    {
        struct sembuf sb;
        sb.sem_num = 0;
        sb.sem_op = 1;
        semop(semd, &sb, 1);
    }
    //print story
    printf("The story so far:\n");
    struct stat fstat;
    stat(FILENAME, &fstat);
    fd = open(FILENAME, O_RDONLY);
    char content[fstat.st_size];
    read(fd, content, fstat.st_size);
    write(STDOUT_FILENO, content, fstat.st_size);
    close(fd);

    //remove shared memory
    shmd = shmget(MEMKEY, SEG_SIZE, 0644);
    if (shmd < 0)
    {
        printf("error - cannot access shared memory: %s\n", strerror(errno));
        return errno;
    }
    else
    {
        if (shmctl(shmd, IPC_RMID, 0))
        {
            printf("error - cannot remove shared memory: %s\n", strerror(errno));
        }
        else
        {
            printf("shared memory removed\n");
        }
    }
    //remove file
    if (remove(FILENAME))
    {
        printf("error - cannot remove file: %s\n", strerror(errno));
        return errno;
    }
    else
    {
        printf("file removed\n");
    }
    //remove semaphore
    if (semctl(semd,IPC_RMID, 0))
    {
        printf("error - cannot remove semaphore: %s\n", strerror(errno));
        return errno;
    }
    else
    {
        printf("semaphore removed\n");
    }
    return 0;
}