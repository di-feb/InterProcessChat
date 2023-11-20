# ClientServer_OS3

This project is about a __messaging application__ consisting of two interactive processes.  

## This project consists of the following folders

- __dataset:__ Contains 2 files, __shm_id.txt__ in which the ids from the shared memories are stored.  
    Αnd __sample2.txt__ that is used for testing certain functions.
- __include:__ Contains the __.h__ files
- __src:__ Contains the __source code__.
  - __process_a.c, process_b.c:__  The two main functions of this project.
  - __modules.c__ The rest of the helper functions.  
- __tests:__ Contains __unit-tests__ for certain functions

## How to run it

- To __run the tests__, move into the __tests__ directory and write:
  - __make run__: For simple execution
  - __make valgrind:__ To run the project using valgrind


- To __run the main function__, move into the __src__ directory and write:
  - __make run_process_a or make run_process_b:__ For simple execution
  - __make valgrind_process_a or make valgrind_process_b:__ To run the project with valgrind  

- **Every process (process_a, process_b) must run into a different machine (Terminal)**
