#pragma once // #include only one time

// Types they are used in many modules

// Use of type "bool" for variables that only take values of true / false
#include <stdbool.h>

// Pointer to an object of any type. It is just more readable than the "void*"
typedef void* Pointer;

// unsigned int, for brevity
typedef unsigned int uint;

// Pointer to a function that compares 2 elements a and b and returns:
// < 0  if a < b
//   0  if a and b are equivalent (_not_ necessarily equal)
// > 0  if a > b
typedef int (*CompareFunc)(Pointer a, Pointer b);

// Pointer to a function that destroys an element "value"
typedef void (*DestroyFunc)(Pointer value);