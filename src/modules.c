#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <dirent.h>
#include <signal.h>
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
    shared_memory->segments_counter = 0;
    return shared_memory;
}

void destroy_segment(SharedMemory shared_memory)
{
    free(shared_memory);
}

// Initializes the shared memory and the semaphores
int initialize_structures(SharedMemory *shared_memory, key_t key)
{
    // Make shared memory segment.
    int shm_id = shmget(key, sizeof(**shared_memory) + MAX_SEGMENTS * MAX_MESSAGE_SEGMENT_SIZE, IPC_CREAT | SHM_PERM);
    if (shm_id == -1)
        perror("Creation of the segment.");
    else
        printf("Allocated shared memory with id: %d.\n", shm_id);

    // Attach the segment.
    if ((*shared_memory = (SharedMemory)shmat(shm_id, NULL, 0)) == (void *)-1)
    {
        perror("Attachment.");
        exit(1);
    } // Handle the error case.
    else
        printf("Attached segment with id: %d.\n", shm_id);

    // Initialize the semaphores
    if (sem_init(&(*shared_memory)->message_full_lock, SEM_PROCESS_SHARED, 0) != 0)
    {
        perror("Couldn't initialize semaphore named message_full_lock.");
        exit(1);
    }
    printf("The semaphore named:message_full_lock has been initialized.\n");

    if (sem_init(&(*shared_memory)->message_empty_lock, SEM_PROCESS_SHARED, 1) != 0)
    {
        perror("Couldn't initialize semaphore named message_empty_lock.");
        exit(1);
    }
    printf("The semaphore named:message_empty_lock has been initialized.\n");

    if (sem_init(&(*shared_memory)->mutex, SEM_PROCESS_SHARED, 1) != 0)
    {
        perror("Couldn't initialize semaphore named mutex.");
        exit(1);
    }
    printf("The semaphore named:mutex has been initialized.\n");

    (*shared_memory)->segments_counter = 0;

    return shm_id;
}

// Deallocates the shared memory and the semaphores
void destroy_structures(SharedMemory shared_memory, int shm_id)
{
    // Deallocate the semaphores
    if (sem_destroy(&shared_memory->mutex) != 0)
    {
        perror("Couldn't destroy semaphore named mutex.");
        exit(1);
    }
    printf("The semaphore named:mutex has been destroyed.\n");

    if (sem_destroy(&shared_memory->message_full_lock) != 0)
    {
        perror("Couldn't destroy semaphore named message_full_lock.");
        exit(1);
    }
    printf("The semaphore named:message_full_lock has been destroyed.\n");

    if (sem_destroy(&shared_memory->message_empty_lock) != 0)
    {
        perror("Couldn't destroy semaphore named message_empty_lock.");
        exit(1);
    }
    printf("The semaphore named:message_empty_lock has been destroyed.\n");

    // Detach the segment.
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
void store_shm_id(int shm_id_1, int shm_id_2)
{
    // Write shm_id to a file
    FILE *fp = fopen("../dataset/shm_id.txt", "w");
    if (fp == NULL)
    {
        perror("Error opening file.");
        exit(1);
    }
    fprintf(fp, "%d\n", shm_id_1);
    fprintf(fp, "%d", shm_id_2);
    printf("Stored shared memory ids: %d, %d with success.\n", shm_id_1, shm_id_2);
    fclose(fp);
}

// Retrieves the shared memory id from a file
void retrieve_shm_id(int *shm_id_1, int *shm_id_2)
{
    // Read shm_id from a file
    FILE *fp = fopen("../dataset/shm_id.txt", "r");
    if (fp == NULL)
    {
        perror("Error opening file.");
        exit(1);
    }
    fscanf(fp, "%d", shm_id_1);
    fscanf(fp, "%d", shm_id_2);

    fclose(fp);
}

// Copies n characters from src to dest
// and puts a null terminator at the end
char *copy_n_chars(char *dest, const char *src, size_t n)
{
    size_t i;
    for (i = 0; i < n && src[i] != '\0'; i++)
    {
        dest[i] = src[i];
    }
    dest[i] = '\0';
    return dest;
}

// Copies n characters from file to dest
// and puts a null terminator at the end
char *copy_n_chars_from_file(char *dest, FILE *file, size_t n)
{
    size_t i;
    int c;
    for (i = 0; i < n && (c = fgetc(file)) != EOF; i++)
    {
        dest[i] = c;
    }
    dest[i] = '\0';
    return dest;
}

// Splits the message into segments of 15 characters
void write_message(SharedMemory shared_memory, char *message)
{
    while (*message)
    {
        char *message_segment = malloc(MAX_MESSAGE_SEGMENT_SIZE + 1);
        copy_n_chars(message_segment, message, MAX_MESSAGE_SEGMENT_SIZE);

        // If we read all the message
        if (*message_segment == '\0')
        {
            free(message_segment);
            break;
        }
        // Copy the message segment to the shared memory
        copy_n_chars(shared_memory->message[shared_memory->segments_counter], message_segment, strlen(message_segment));
        shared_memory->segments_counter++;

        // Check if the message is #BYE#
        if (strcmp(message_segment, "#BYE#") == 0)
        {
            free(message_segment);

            // We set the message_full_lock to 1 so the reader can read
            sem_post(&shared_memory->message_full_lock);
            pthread_exit(NULL);
        }

        message += strlen(message_segment);
        free(message_segment);
    }
}

// Splits the file into segments of 15 characters
void segment_file(SharedMemory shared_memory, char *filename)
{
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        perror("Error opening file.");
        return;
    }

    while (!feof(file))
    {
        char *message_segment = malloc(MAX_MESSAGE_SEGMENT_SIZE + 1);
        copy_n_chars_from_file(message_segment, file, MAX_MESSAGE_SEGMENT_SIZE);
        // If we read all the file
        if (*message_segment == '\0')
        {
            free(message_segment);
            break;
        }

        // Copy the message segment to the shared memory
        copy_n_chars(shared_memory->message[shared_memory->segments_counter], message_segment, strlen(message_segment));
        shared_memory->segments_counter++;

        free(message_segment);
    }

    fclose(file);
}

// It handles the message.
// Check every segment of it and if it is #BYE# then
// it stops printing the message and returns 1.
// Otherwise it returns 0.
int read_message(SharedMemory shared_memory)
{
    int res = 0;
    char full_message[MAX_MESSAGE_SIZE] = "";
    for (int i = 0; i < shared_memory->segments_counter; i++)
        copy_n_chars(full_message + strlen(full_message),
                     shared_memory->message[i], strlen(shared_memory->message[i]));

    if (strcmp(full_message, "#BYE#") == 0)
        res = 1;

    printf("\033[1;36mFriend: \033[0m%s\n", full_message);
    fflush(stdout); // Manually flush the buffer because we don't have a newline character

    return res;
}

void empty_message(SharedMemory shared_memory)
{
    while (shared_memory->segments_counter > 0)
    {
        shared_memory->message[shared_memory->segments_counter - 1][0] = '\0';
        shared_memory->segments_counter--;
    }
}

// The thread code that is responsible for receiving message
void *receive_message(void *data)
{
    SharedMemory shared_memory = (SharedMemory)data;
    while (1)
    {
        // The reader waits the writer to finish
        sem_wait(&shared_memory->message_full_lock);
        int res = read_message(shared_memory);

        // if the res is equal to 1 then the message was #BYE#
        // so we need to stop the chatting and exit the thread
        if (res)
        {
            // We set the message_empty_lock to 0 so the writer can write
            sem_post(&shared_memory->message_empty_lock);
            pthread_exit(NULL);
        }

        empty_message(shared_memory);

        // We set the message_empty_lock to 0 so the writer can write
        sem_post(&shared_memory->message_empty_lock);
    }
}

// The thread code that is responsible for sending message
void *send_message(void *data)
{
    SharedMemory shared_memory = (SharedMemory)data;

    while (1)
    {
        // The writer waits the reader to finish
        sem_wait(&shared_memory->message_empty_lock);

        // The message the user wants to send
        char message[MAX_MESSAGE_SIZE + 1];

        // Read the message from the user
        fgets(message, MAX_MESSAGE_SIZE, stdin);

        // Remove the newline character if it's present
        char *newline_position = strchr(message, '\n');
        if (newline_position != NULL)
            *newline_position = '\0';

        // Now the writer can write
        write_message(shared_memory, message);

        // We set the message_full_lock to 1 so the reader can read
        sem_post(&shared_memory->message_full_lock);
    }
}