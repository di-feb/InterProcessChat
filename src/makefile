# paths
INCLUDE = include/

# compiler
CC = gcc

# Compile options.
CFLAGS = -Wall -g 
LDFLAGS = -lm -lpthread

# files .o
process_a_OBJS = process_a.o modules.o
process_b_OBJS = process_b.o modules.o

# The executable program
process_a_EXEC = process_a
process_b_EXEC = process_b

# Args
process_a_ARGS = 
process_b_ARGS =

$(process_a_EXEC): $(process_a_OBJS)
	$(CC) $(process_a_OBJS) -o $(process_a_EXEC) $(LDFLAGS)
	rm $(RMFLAGS) $(process_a_OBJS)

$(process_b_EXEC): $(process_b_OBJS)
	$(CC) $(process_b_OBJS) -o $(process_b_EXEC) $(LDFLAGS)
	rm $(RMFLAGS) $(process_b_OBJS)

clean_process_a:
	rm $(process_a_OBJS) $(process_a_EXEC)

clean_process_b:
	rm $(process_b_OBJS) $(process_b_EXEC)

clean: 
	rm -f *.o *.d $(process_a_EXEC) $(process_b_EXEC)

run_process_a: $(process_a_EXEC)
	./$(process_a_EXEC) $(process_a_ARGS)

run_process_b: $(process_b_EXEC)
	./$(process_b_EXEC) $(process_b_ARGS)

valgrind_process_a: $(process_a_EXEC)
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./$(process_a_EXEC) $(process_a_ARGS)

valgrind_process_b: $(process_b_EXEC)
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./$(process_b_EXEC) $(process_b_ARGS)