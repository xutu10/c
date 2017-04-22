CC          = /usr/bin/gcc
CFLAGS      = -Wall -g -O0 -std=gnu99
LDFLAGS     = -L. -lvfilesystem -lglib-2.0
INCLUDES    = -Iinclude -I../3/include
BINARY      = fileserver

all: $(BINARY)

$(BINARY): vfilesystem_server_marshaller.o main.o
	$(CC) $^ -o $@ $(LDFLAGS)

%.o: ./src/%.c
	$(CC) $(INCLUDES) $(CFLAGS) -c $< -o $@

clean:
	@rm -rf *.o $(BINARY)

