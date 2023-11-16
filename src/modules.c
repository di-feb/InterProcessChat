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
#include <time.h>
#include <wait.h>
#include <pthread.h>
#include "../include/ADTVector.h"
#include "../include/headers.h"

// Mallocs new memory for a string and
// copies the contents of the given string
char *create_string(char *str)
{
    char *new_string = malloc(strlen(str) + 1);
    strcpy(new_string, str);
    return new_string;
}

SharedMemory create_segment()
{
    SharedMemory shared_memory = malloc(sizeof(*shared_memory));
    shared_memory->message = vector_create(0, free);
    return shared_memory;
}

void destroy_segment(SharedMemory shared_memory)
{
    vector_destroy(shared_memory->message);
    free(shared_memory);
}

// Initializes the shared memory and the semaphores
int initialize_structures(SharedMemory shared_memory)
{
    // Make shared memory segment.
    int shm_id = shmget(IPC_PRIVATE, 5 * sizeof(*shared_memory), IPC_CREAT | SHM_PERM);
    if (shm_id == -1)
        perror("Creation of the segment.");
    else
        printf("Allocated shared memory with id: %d.\n", shm_id);

    // Attach the segment.
    if ((shared_memory = (SharedMemory)shmat(shm_id, NULL, 0)) == (void *)-1)
    {
        perror("Attachment.");
        exit(1);
    } // Handle the error case.
    else
        printf("Attached segment with id: %d.\n", shm_id);

    // Initialize the semaphores
    if (sem_init(&shared_memory->writer_lock, SEM_PROCESS_SHARED, 1) != 0)
    {
        perror("Couldn't initialize semaphore named writer_lock.");
        exit(1);
    }
    printf("The semaphore named:writer_lock has been initialized.\n");

    if (sem_init(&shared_memory->reader_lock, SEM_PROCESS_SHARED, 0) != 0)
    {
        perror("Couldn't initialize semaphore named reader_lock.");
        exit(1);
    }
    printf("The semaphore named:reader_lock has been initialized.\n");

    return shm_id;
}

// Deallocates the shared memory and the semaphores
void destroy_structures(SharedMemory shared_memory, int shm_id)
{
    // Deallocate the semaphores
    if (sem_destroy(&shared_memory->writer_lock) != 0)
    {
        perror("Couldn't destroy semaphore named writer_lock.");
        exit(1);
    }
    printf("The semaphore named:writer_lock has been destroyed.\n");

    if (sem_destroy(&shared_memory->reader_lock) != 0)
    {
        perror("Couldn't destroy semaphore named reader_lock.");
        exit(1);
    }
    printf("The semaphore named:reader_lock has been destroyed.\n");

    int err = shmdt((void *)shared_memory);
    if (err == -1)
        perror("Detachment.");
    else
        printf("Detachment of Shared Segment with success. Code:%d\n", err);

    // Remove segment.
    err = shmctl(shm_id, IPC_RMID, 0);
    if (err == -1)
        perror("Removal.");

    else
        printf("Removed segment with success. Code %d\n", err);
}

// Stores the shared memory id so process B can retrieve it
// into a file
void store_shm_id(int shm_id)
{
    // Write shm_id to a file
    FILE *fp = fopen("shm_id.txt", "w");
    if (fp == NULL)
    {
        perror("Error opening file.");
        exit(1);
    }
    fprintf(fp, "%d", shm_id);
    fclose(fp);
}

// Retrieves the shared memory id from a file
void retrieve_shm_id(int *shm_id)
{
    // Read shm_id from a file
    FILE *fp = fopen("shm_id.txt", "r");
    if (fp == NULL)
    {
        perror("Error opening file.");
        exit(1);
    }
    fscanf(fp, "%d", shm_id);
    fclose(fp);
}

char *copy_n_chars(char *dest, const char *src, size_t n)
{
    size_t i;
    for (i = 0; i < n && src[i]; i++) {
        dest[i] = src[i];
    }
    dest[i] = '\0';
    return dest;
}

// Splits the message into segments of 15 characters
void segment_message(SharedMemory shared_memory, char *message)
{
    while (*message)
    {
        char *message_segment = malloc(MAX_MESSAGE_SIZE + 1);
        copy_n_chars(message_segment, message, MAX_MESSAGE_SIZE);
        // If we read all the message
        if (*message_segment == '\0')
        {
            free(message_segment);
            break;
        }
        vector_insert_last(shared_memory->message, create_string(message_segment));
        message += strlen(message_segment);
        free(message_segment);
    }
}

// Splits the file into segments of 15 characters
void segment_file(SharedMemory shared_memory, char* filename)
{
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file.");
        return;
    }

    while (!feof(file))
    {
        char *message_segment = malloc(MAX_MESSAGE_SIZE);
        int chars_read = fscanf(file, "%15s", message_segment);
        // If we read all the file
        if (chars_read <= 0) {
            free(message_segment);
            break;
        }
        vector_insert_last(shared_memory->message, create_string(message_segment));
        free(message_segment);
    }

    fclose(file);
}

// The thread code that is responsible for receiving messages
void *receive_message(void *data)
{
    return NULL;
}

// The thread code that is responsible for sending messages
void *send_message(void *data)
{
    return NULL;
}