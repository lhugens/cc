# Compiler and compiler flags
CC = mpicc
CFLAGS = -lm

# Source files and target executable
SRCS = fox.c 
OBJS = $(SRCS:.c=.o)
TARGET = fox

# Default target
all: $(TARGET)

# Compile source files to object files
%.o: %.c
	$(CC) $< -o $@ $(CFLAGS)

# Link object files to create the executable
$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $@ $(CFLAGS)

# Clean up generated files
clean:
	rm -f $(OBJS) $(TARGET)
