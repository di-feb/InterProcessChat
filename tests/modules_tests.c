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


TEST_LIST = {
    {"create_string", test_create_string},
    {"store_retrieve_shm_id", store_retrieve_shm_id},
    {"test_copy_n_chars", test_copy_n_chars},
    {"test_copy_n_chars_from_file", test_copy_n_chars_from_file},
    {NULL, NULL}};