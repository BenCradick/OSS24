CC = gcc
CFLAGS = -Wall -Werror

# Source files
OSS_SRC = oss.c
USER_SRC = user.c

# Object files
OSS_OBJ = $(OSS_SRC:.c=.o)
USER_OBJ = $(USER_SRC:.c=.o)

# Header files
OSS_HEADERS = oss.h
USER_HEADERS = user.h

# Executables
OSS_EXECUTABLE = oss
USER_EXECUTABLE = user

# Default target
all: $(OSS_EXECUTABLE) $(USER_EXECUTABLE)

# Compile OSS program
$(OSS_EXECUTABLE): $(OSS_OBJ)
	$(CC) $(CFLAGS) -o $@ $^

# Compile USER program
$(USER_EXECUTABLE): $(USER_OBJ)
	$(CC) $(CFLAGS) -o $@ $^

# Compile source files to object files
%.o: %.c $(OSS_HEADERS) $(USER_HEADERS)
	$(CC) $(CFLAGS) -c -o $@ $<

# Clean up
clean:
	rm -f $(OSS_OBJ) $(USER_OBJ) $(OSS_EXECUTABLE) $(USER_EXECUTABLE)
