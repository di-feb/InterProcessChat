#pragma once

#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <dirent.h>
#include <signal.h>
#include <semaphore.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h> 
#include "headers.h"
#include <time.h>
#include <wait.h>
#include <pthread.h>

#define SHM_SIZE 1024
#define SHM_PERM 0666
#define SEM_PROCESS_SHARED 1
#define MAXLINESIZE 64000
#define MIN_PARTITION 1
#define MIN_NUM_lINES 100
#define MAX_CHILDS_WAITING 30 
#define MAX_SEGS 1000

typedef struct shared_memory* SharedMemory;
typedef sem_t Semaphore;

struct shared_memory{
    
};