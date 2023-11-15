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
#define MAX_MESSAGE_SIZE 15
#define MAX_MESSAGES 100


typedef struct shared_memory* SharedMemory;
typedef sem_t Semaphore;

struct shared_memory{
    char message[MAX_MESSAGES][MAX_MESSAGE_SIZE];
    Semaphore writer_lock;
    Semaphore reader_lock;

};

// Initializes the shared memory and the semaphores
int initialize_structures(SharedMemory shared_memory);

// Deallocates the shared memory and the semaphores
void destroy_structures();

// Stores the shared memory id
void store_shm_id(int shm_id);

// Retrieves the shared memory id
void retrieve_shm_id(int* shm_id);

// Sends a new message
void* send_message(void*);

// Receives a new message
void* receive_message(void*);

