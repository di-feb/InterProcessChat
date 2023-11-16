#include "../include/headers.h"

// The process A is responsible for initializing the shared memory and the semaphores
// and deallocating them when the program finishes. 
int main(int argc, char** argv)
{
    SharedMemory shared_memory;
    // Initialize the shared memory and the semaphores
    int shm_id = initialize_structures(shared_memory);

    // Store the shared memory id so process B can retrieve it
    store_shm_id(shm_id);

    // Create the sender and receiver threads
    pthread_t sender_thread, receiver_thread;
    pthread_create(&sender_thread, NULL, send_message, NULL);
    pthread_create(&receiver_thread, NULL, receive_message, NULL);

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


    // Deallocate the shared memory and the semaphores
    destroy_structures(shared_memory, shm_id);

    return 0;
}
// vector
// oxi fork()
// den exw katalabei gia user na stelnei
// υπαρχει μαξ χαρακτηρες στα μνματα που θα στελνονται;
