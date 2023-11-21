#pragma once

#include <stdio.h>     // For FILE
#include <stdlib.h>    // For malloc
#include <semaphore.h> // For sem_t
#include <time.h>      // For clock_t

#define SHM_SIZE 1024
#define SHM_PERM 0666
#define SEM_PROCESS_SHARED 1
#define MAX_SEGMENTS 1000
#define MAX_MESSAGE_SEGMENT_SIZE 15
#define MAX_MESSAGE_SIZE 4096
#define PROCESS_A_KEY 200
#define PROCESS_B_KEY 300

typedef struct shared_memory *SharedMemory;
typedef sem_t Semaphore;

struct shared_memory
{
    char message[MAX_MESSAGE_SEGMENT_SIZE + 1];                 // The message the user wants to send
    int message_end;                                            // If its value is 1 then the message has ended
    int segments_counter;                                       // The number of segments the message has
    int total_messages_sent;                                    // The total number of messages the user has sent
    int total_messages_received;                                // The total number of messages the user has received
    int total_segments;                                         // The total number of segments the user's messages has been split into
    double total_wait_time;                                     // The total time the user has waited for the other process to read the message
    clock_t write_end_time;                                     // The time when the user ends writing a message
    clock_t read_start_time;                                    // The time when the user reads the first segment of a message
    Semaphore message_full_lock;                                // The reader waits for the message to be written
    Semaphore message_empty_lock;                               // The writer waits for the message to be read
    Semaphore mutex;                                            // Ensures mutual exclusion between the two processes
};

// Mallocs new memory for a string and
// copies the contents of the given string
char *create_string(char *str);

// Creates the shared memory segment
SharedMemory create_segment();

// Deallocates the shared memory segment
void destroy_segment(SharedMemory shared_memory);

// Initializes the shared memory and the semaphores
int initialize_structures(SharedMemory *shared_memory, key_t key);

// Destroy the shared memory segment and the semaphores
void destroy_structures();

// Stores the shared memory id
void store_shm_id(int shm_id_1, int shm_id_2);

// Retrieves the shared memory id
void retrieve_shm_id(int *shm_id_1, int *shm_id_2);

// Copies n characters from src to dest
// and puts a null terminator at the end
// Returns how many characters were copied
int copy_n_chars(char *dest, const char *src, size_t n);

// Copies n characters from file to dest
// and puts a null terminator at the end
char *copy_n_chars_from_file(char *dest, FILE *file, size_t n);

// Splits the message into parts of 15 characters
void write_message(SharedMemory shared_memory, char *message);

// Splits the file into parts of 15 characters
void segment_file(SharedMemory shared_memory, char *filename);

// It handles the message.
// Check every segment of it and if it is #BYE# then
// it stops printing the message and returns 1.
// Otherwise it returns 0.
int read_message(SharedMemory shared_memory);

// Empty all the segments of the message
void empty_message(SharedMemory shared_memory);

// Sends a new message
void *send_message(void *data);

// Receives a new message
void *receive_message(void *data);
