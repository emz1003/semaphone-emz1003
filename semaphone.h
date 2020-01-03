#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <errno.h>

#define MEMKEY 24606
#define SEMKEY 24607
#define SEG_SIZE 2048
#define FILENAME "story"

// union semun {
//     int val;               /* Value for SETVAL */
//     struct semid_ds *buf;  /* Buffer for IPC_STAT, IPC_SET */
//     unsigned short *array; /* Array for GETALL, SETALL */
//     struct seminfo *__buf; /* Buffer for IPC_INFO
//                                            (Linux-specific) */
// };

int cre(int semd, int shmd, int fd);
int rem(int semd, int shmd, int fd);