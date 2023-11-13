# paths
INCLUDE = include/

# compiler
CC = gcc

# Compile options.
CFLAGS = -Wall -g 
LDFLAGS = -lm -lpthread

# files .o
OBJS = chat.o reader.o writer.o modules.o 

# The executable program
EXEC = chat

# Args
ARGS = data.txt 10 2 250


$(EXEC): $(OBJS)
	$(CC) $(OBJS) -o $(EXEC) $(LDFLAGS)
	rm $(RMFLAGS) $(OBJS)

clean:
	rm -f $(OBJS) $(EXEC)

run: $(EXEC)
	./$(EXEC) $(ARGS)

valgrind: $(EXEC)
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./$(EXEC) $(ARGS)