CROSS=arm-linux-

all: key

key:key.c
	$(CROSS)gcc -o key key.c
	$(CROSS)strip key
clean:
	@rm -vf key *.o *~
