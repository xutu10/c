CC          = /usr/bin/gcc
CFLAGS      = -Wall -g -O0
INCLUDES    = -Iinclude

BINARY      = filesystem
LIBRARY     = libvfilesystem.a

$(BINARY): main.o $(LIBRARY)
	$(CC) $^ -o $@ $(LDFLAGS)

help:
	@echo "Targets:"
	@echo "$(BINARY) - creates the file system library and the test executeable"
	@echo "$(LIBRARY) - creates the system library"
	@echo "clean - removes the objects"

$(LIBRARY): vfilesystem_api.o
	$(AR) rs $@ $^

%.o: ./src/%.c
	$(CC) $(INCLUDES) $(CFLAGS) -c $< -o $@

clean:
	@rm -rf *.o $(BINARY) $(LIBRARY)
