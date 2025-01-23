# Compiler and Flags
CC = clang
CFLAGS = -g -Wall

# Formatting Tool
FORMAT = clang-format
FORMAT_FLAGS = -i

# Source Files
SRCS = main.c
OBJS = $(SRCS:.c=.o)
EXEC = main

# Default Target: Compile Program
all: $(EXEC)

# Compile Executable
$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) -o $(EXEC) $(OBJS)

# Compile Object Files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Run Clang-Format
format:
	$(FORMAT) $(FORMAT_FLAGS) $(SRCS)

# Clean Build Files
clean:
	rm -f $(OBJS) $(EXEC)
