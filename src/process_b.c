#include "../include/headers.h"
#include <sys/shm.h>
#include <pthread.h>

int main(int argc, char** argv)
{

    // Retrieve the shared memory id so it can be attached
    // to the memory segment
    int shm_id_1, shm_id_2;
    retrieve_shm_id(&shm_id_1, &shm_id_2);
    
    // Attach the segment.
    
    SharedMemory shared_memory_a = (SharedMemory)shmat(shm_id_1, NULL, 0);
    if(shared_memory_a == (void *)-1)
    {
        perror("Attachment.");
        exit(1);
    } // Handle the error case.
    else
        printf("Attached segment with id: %d.\n", shm_id_1);
    SharedMemory shared_memory_b = (SharedMemory)shmat(shm_id_2, NULL, 0);
    if(shared_memory_b == (void *)-1)
    {
        perror("Attachment.");
        exit(1);
    } // Handle the error case.
    else
        printf("Attached segment with id: %d.\n", shm_id_2);

    if(shared_memory_a->message == NULL)
    {
        perror("Error creating the vector.");
        exit(1);
    }

    if(shared_memory_b->message == NULL)
    {
        perror("Error creating the vector.");
        exit(1);
    }

    // Create the sender and receiver threads
    pthread_t sender_thread, receiver_thread;
    pthread_create(&sender_thread, NULL, send_message, (void*)shared_memory_b);
    pthread_create(&receiver_thread, NULL, receive_message, (void*)shared_memory_a);

    // Wait for the threads to finish
    if(pthread_join(sender_thread, NULL) != 0)
    {
        perror("Error joining sender thread.");
        exit(1);
    }
    if(pthread_join(receiver_thread, NULL) != 0)
    {
        perror("Error joining receiver thread.");
        exit(1);
    }
    
    return 0;
}