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
process_a_ARGS = data.txt 10 2 250
process_b_ARGS =

$(process_a_EXEC): $(process_a_OBJS)
	$(CC) $(process_a_OBJS) -o $(process_a_EXEC) $(LDFLAGS)
	rm $(RMFLAGS) $(process_a_OBJS)

$(process_b_EXEC): $(process_b_OBJS)
	$(CC) $(process_b_OBJS) -o $(process_b_EXEC) $(LDFLAGS)
	rm $(RMFLAGS) $(process_b_OBJS)

clean process_a:
	rm -f $(process_a_OBJS) $(process_a_EXEC)

clean process_b:
	rm -f $(process_b_OBJS) $(process_b_EXEC)

run process_a: $(process_a_EXEC)
	./$(process_a_EXEC) $(process_a_ARGS)

run process_b: $(process_b_EXEC)
	./$(process_b_EXEC) $(process_b_ARGS)

valgrind process_a: $(process_a_EXEC)
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./$(process_a_EXEC) $(process_a_ARGS)

valgrind process_b: $(process_b_EXEC)
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./$(process_b_EXEC) $(process_b_ARGS)