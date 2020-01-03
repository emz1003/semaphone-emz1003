#include "semaphone.h"

int main()
{
    int fd = open(FILENAME, O_WRONLY | O_APPEND);
    if (fd < 0)
    {
        printf("file not found\n");
        return 1;
    }
    int semd;
    int shmd;
    char *story;

    printf("trying to get in\n");

    semd = semget(SEMKEY, 1, 0);
    struct sembuf sb;
    sb.sem_num = 0;
    sb.sem_op = -1;
    sb.sem_flg = SEM_UNDO;
    semop(semd, &sb, 1);
    shmd = shmget(MEMKEY, SEG_SIZE, 0);
    story = shmat(shmd, 0, 0);

    printf("Last addition: %s", story);
    printf("Your addition: ");
    fgets(story, SEG_SIZE, stdin);
    write(fd, story, SEG_SIZE);
    close(fd);
    shmdt(story);
    sb.sem_op = 1;
    semop(semd, &sb, 1);

    return 0;
}