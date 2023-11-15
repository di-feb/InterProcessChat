#include "headers.h"

int main(int argc, char** argv)
{

    // Retrieve the shared memory id so it can be attached
    // to the memory segment
    int shm_id;
    retrieve_shm_id(&shm_id);
    
    // Attach the segment.
    SharedMemory shared_memory = (SharedMemory)shmat(shm_id, NULL, 0);

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
    
    return 0;
}