#include "headers.h"

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

void store_shm_id(int shm_id)
{
    // Write shm_id to a file
    FILE* fp = fopen("shm_id.txt", "w");
    if (fp == NULL) {
        perror("Error opening file.");
        exit(1);
    }
    fprintf(fp, "%d", shm_id);
    fclose(fp);
}

void retrieve_shm_id(int* shm_id)
{
    // Read shm_id from a file
    FILE* fp = fopen("shm_id.txt", "r");
    if (fp == NULL) {
        perror("Error opening file.");
        exit(1);
    }
    fscanf(fp, "%d", shm_id);
    fclose(fp);
}

void *receive_message(void *data)
{
}

void *send_message(void *data)
{
}