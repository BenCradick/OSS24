CC = gcc
CFLAGS = -Wall -Wextra


OSS_SRC = oss.c
USER_SRC = user.c
WORKER_SRC = worker.c


OSS_OBJ = $(OSS_SRC:.c=.o)
USER_OBJ = $(USER_SRC:.c=.o)
WORKER_OBJ = $(WORKER_SRC:.c=.o)


USER_HEADERS = user.h


OSS_EXECUTABLE = oss
USER_EXECUTABLE = user
WORKER_EXECUTABLE = worker

.PHONY: all
all: $(OSS_EXECUTABLE) $(USER_EXECUTABLE) $(WORKER_EXECUTABLE)


$(OSS_EXECUTABLE): $(OSS_OBJ)
	$(CC) $(CFLAGS) -o $@ $^ -lrt


$(USER_EXECUTABLE): $(USER_OBJ)
	$(CC) $(CFLAGS) -o $@ $^ -lrt

$(WORKER_EXECUTABLE): $(WORKER_OBJ)
	$(CC) $(CFLAGS) -o $@ $^ -lrt


%.o: %.c $(OSS_HEADERS) $(USER_HEADERS) $(WORKER_HEADERS)
	$(CC) $(CFLAGS) -c -o $@ $<

.PHONY: clean
clean:
	rm -f $(OSS_OBJ) $(USER_OBJ) $(OSS_EXECUTABLE) $(USER_EXECUTABLE) $(WORKER_OBJ) $(WORKER_EXECUTABLE)
