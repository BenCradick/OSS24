CC = gcc
CFLAGS = -Wall -Werror


OSS_SRC = oss.c
USER_SRC = user.c


OSS_OBJ = $(OSS_SRC:.c=.o)
USER_OBJ = $(USER_SRC:.c=.o)


USER_HEADERS = user.h


OSS_EXECUTABLE = oss
USER_EXECUTABLE = user


all: $(OSS_EXECUTABLE) $(USER_EXECUTABLE)


$(OSS_EXECUTABLE): $(OSS_OBJ)
	$(CC) $(CFLAGS) -o $@ $^


$(USER_EXECUTABLE): $(USER_OBJ)
	$(CC) $(CFLAGS) -o $@ $^


%.o: %.c $(OSS_HEADERS) $(USER_HEADERS)
	$(CC) $(CFLAGS) -c -o $@ $<


clean:
	rm -f $(OSS_OBJ) $(USER_OBJ) $(OSS_EXECUTABLE) $(USER_EXECUTABLE)
