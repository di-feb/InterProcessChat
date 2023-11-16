#pragma once

#include <semaphore.h> // For sem_t
#include "ADTVector.h" // For Vector

#define SHM_SIZE 1024
#define SHM_PERM 0666
#define SEM_PROCESS_SHARED 1
#define MAX_MESSAGE_SIZE 15
#define MAX_MESSAGES 100


typedef struct shared_memory* SharedMemory;
typedef sem_t Semaphore;

struct shared_memory{
    Vector message;
    Semaphore writer_lock;
    Semaphore reader_lock;

};

// Mallocs new memory for a string and
// copies the contents of the given string
char *create_string(char *str);

// Creates the shared memory segment
SharedMemory create_segment();

// Deallocates the shared memory segment
void destroy_segment(SharedMemory shared_memory);

// Initializes the shared memory and the semaphores
int initialize_structures(SharedMemory shared_memory);

// Destroy the shared memory segment and the semaphores
void destroy_structures();

// Stores the shared memory id
void store_shm_id(int shm_id);

// Retrieves the shared memory id
void retrieve_shm_id(int* shm_id);

// Splits the message into parts of 15 characters
void segment_message(SharedMemory shared_memory, char* message);

// Splits the file into parts of 15 characters
void segment_file(SharedMemory shared_memory, char* filename);

// Sends a new message
void* send_message(void*);

// Receives a new message
void* receive_message(void*);

