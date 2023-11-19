#include <stdio.h>
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
    int shm_id_1 = 5;
    int shm_id_2 = 10;

    store_shm_id(shm_id_1, shm_id_2);

    int retrieved_shm_id_1, retrieved_shm_id_2;
    retrieve_shm_id(&retrieved_shm_id_1, &retrieved_shm_id_2);

    TEST_CHECK(shm_id_1 == retrieved_shm_id_1);
    TEST_CHECK(shm_id_2 == retrieved_shm_id_2);
}

void test_copy_n_chars(){
    char *str = "Hello World";
    char *new_string = malloc(strlen(str) + 1);
    copy_n_chars(new_string, str, 5);
    TEST_CHECK(strcmp(new_string, "Hello") == 0);
    TEST_CHECK(strlen(new_string) == 5);
    free(new_string);
}

void test_copy_n_chars_from_file(){
    FILE *file = fopen("../dataset/sample2.txt", "r");
    char *new_string = malloc(9);
    copy_n_chars_from_file(new_string, file, 8);
    TEST_CHECK(strcmp(new_string, "Aeque en") == 0);
    TEST_CHECK(strlen(new_string) == 8);
    free(new_string);
    fclose(file);
}

void test_write_message()
{
    // Create the memory segment
    SharedMemory shared_memory = create_segment();

    // Lets say we have a message of 15 characters
    char *str = "Hello World!!!!";
    write_message(shared_memory, str);

    // The string should be stored in the vector
    // and it should fit whole into its first place
    TEST_CHECK(strcmp(shared_memory->message[0] , str) == 0);

    // Lets say we have a message of 30 characters
    str = "Hello World$$$$Hello World????";
    write_message(shared_memory, str);
    
    // The string should be stored in the vector
    // and it should be split into two parts
    TEST_CHECK(strcmp(shared_memory->message[1], "Hello World$$$$") == 0);
    TEST_CHECK(strcmp(shared_memory->message[2], "Hello World????") == 0);

    // if the string is not divisible by 15
    // we should not have a problem
    str = "Hello World";
    write_message(shared_memory, str);
    TEST_CHECK(strcmp(shared_memory->message[3], str) == 0);

    // One more check
    str = "Hello World$$$$Hello World????F";
    write_message(shared_memory, str);
    
    // The string should be stored in the vector
    // and it should be split into three parts
    TEST_CHECK(strcmp(shared_memory->message[4], "Hello World$$$$") == 0);
    TEST_CHECK(strcmp(shared_memory->message[5], "Hello World????") == 0);
    TEST_CHECK(strcmp(shared_memory->message[6], "F") == 0);

    // Free the memory
    destroy_segment(shared_memory);

}

void test_segment_file(){
    // Create the memory segment
    SharedMemory shared_memory = create_segment();

    char *filename = "../dataset/sample2.txt";
    segment_file(shared_memory, filename);

    TEST_CHECK(strcmp(shared_memory->message[0], "Aeque enim cont") == 0);
    TEST_CHECK(strcmp(shared_memory->message[1], "ingit omnibus f") == 0);
    TEST_CHECK(strcmp(shared_memory->message[shared_memory->segments_counter - 1], "non erit;") == 0);

    // Free the memory
    destroy_segment(shared_memory);
    
}

TEST_LIST = {
    {"create_string", test_create_string},
    // {"initialize_structures", test_initialize_structures},
    {"store_retrieve_shm_id", store_retrieve_shm_id},
    {"test_copy_n_chars", test_copy_n_chars},
    {"test_copy_n_chars_from_file", test_copy_n_chars_from_file},
    {"test_write_message", test_write_message},
    {"test_segment_file", test_segment_file},
    {NULL, NULL}};