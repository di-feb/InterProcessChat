#include "../include/headers.h"
#include <pthread.h>

// The process A is responsible for initializing the shared memory and the semaphores
// and deallocating them when the program finishes. 
int main(int argc, char** argv)
{
    // Create the shared memory segment
    SharedMemory shared_memory_a = NULL;
    SharedMemory shared_memory_b = NULL;
     
    // Initialize the shared memory and the semaphores
    int shm_id_a = initialize_structures(&shared_memory_a, PROCESS_A_KEY);
    int shm_id_b = initialize_structures(&shared_memory_b, PROCESS_B_KEY);

    // Store the shared memory id so process B can retrieve it
    store_shm_id(shm_id_a, shm_id_b);

    printf("\n\n\033[1;32m-----> CHAT APPLICATION <-----\033[0m\n\n");

    // Create the sender and receiver threads
    pthread_t sender_thread, receiver_thread;
    pthread_create(&sender_thread, NULL, send_message, (void*)shared_memory_a);
    pthread_create(&receiver_thread, NULL, receive_message, (void*)shared_memory_b);

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

    printf("\n\n\033[1;32m-----> END OF CHAT APPLICATION <-----\033[0m\n\n");

    printf("\n\n\033[1;33m-----> PROCESS_A STATISTICS <-----\033[0m\n\n");

    printf("Total messages sent: %d\n", shared_memory_a->total_messages_sent);
    printf("Total messages received: %d\n", shared_memory_b->total_messages_received);
    printf("Total segments sent: %d\n", shared_memory_a->total_segments);
    printf("Average segments per message: %f\n", (double)shared_memory_a->total_segments / shared_memory_a->total_messages_sent);
    printf("Average wait time: %f\n", shared_memory_a->total_wait_time / shared_memory_a->total_messages_sent);

    printf("\n\n\033[1;33m-----> END OF PROCESS_A STATISTICS <-----\033[0m\n\n");



    // Deallocate the shared memory and the semaphores
    destroy_structures(shared_memory_a, shm_id_a);
    destroy_structures(shared_memory_b, shm_id_b);
    
    return 0;
}
// vector
// oxi fork()
// den exw katalabei gia user na stelnei
// υπαρχει μαξ χαρακτηρες στα μνματα που θα στελνονται;
