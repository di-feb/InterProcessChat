#include "../include/headers.h"
#include "../include/acutest.h"

void test_create_string()
{
    char *str = "Hello";
    char *new_string = create_string(str);
    TEST_CHECK(strcmp(str, new_string) == 0);
    free(new_string);
}

// void test_initialize_structures()
// {
//     SharedMemory shared_memory;
//     int shm_id = initialize_structures(shared_memory);
//     TEST_CHECK(shm_id != -1);
//     destroy_structures(shared_memory, shm_id);
// }

void store_retrieve_shm_id()
{
    int shm_id = 5;
    store_shm_id(shm_id);

    int retrieved_shm_id;
    retrieve_shm_id(&retrieved_shm_id);

    TEST_CHECK(shm_id == retrieved_shm_id);
}

void test_segment_message()
{
    // Create the memory segment
    SharedMemory shared_memory = create_segment();

    // Lets say we have a message of 15 characters
    char *str = "Hello World!!!!";
    segment_message(shared_memory, str);

    // The string should be stored in the vector
    // and it should fit whole into its first place
    printf("0:%s\n", (char*)vector_get_at(shared_memory->message, 0));
    TEST_CHECK(strcmp((char*)vector_get_at(shared_memory->message, 0), str) == 0);

    // Lets say we have a message of 30 characters
    str = "Hello World$$$$Hello World????";
    segment_message(shared_memory, str);
    
    // The string should be stored in the vector
    // and it should be split into two parts
    printf("1:%s\n", (char*)vector_get_at(shared_memory->message, 1));
    printf("2:%s\n", (char*)vector_get_at(shared_memory->message, 2));
    TEST_CHECK(strcmp((char*)vector_get_at(shared_memory->message, 1), "Hello World$$$$") == 0);
    TEST_CHECK(strcmp((char*)vector_get_at(shared_memory->message, 2), "Hello World????") == 0);

    // if the string is not divisible by 15
    // we should not have a problem
    str = "Hello World";
    segment_message(shared_memory, str);
    printf("3:%s\n", (char*)vector_get_at(shared_memory->message, 3));
    TEST_CHECK(strcmp((char*)vector_get_at(shared_memory->message, 3), str) == 0);

    // One more check
    str = "Hello World$$$$Hello World????F";
    segment_message(shared_memory, str);
    
    // The string should be stored in the vector
    // and it should be split into three parts
    printf("4:%s\n", (char*)vector_get_at(shared_memory->message, 4));
    printf("5:%s\n", (char*)vector_get_at(shared_memory->message, 5));
    printf("6:%s\n", (char*)vector_get_at(shared_memory->message, 6));
    TEST_CHECK(strcmp((char*)vector_get_at(shared_memory->message, 4), "Hello World$$$$") == 0);
    TEST_CHECK(strcmp((char*)vector_get_at(shared_memory->message, 5), "Hello World????") == 0);
    TEST_CHECK(strcmp((char*)vector_get_at(shared_memory->message, 6), "F") == 0);

    // Free the memory
    destroy_segment(shared_memory);

}

TEST_LIST = {
    {"create_string", test_create_string},
    // {"initialize_structures", test_initialize_structures},
    {"store_retrieve_shm_id", store_retrieve_shm_id},
    {"test_segment_message", test_segment_message},
    {NULL, NULL}};