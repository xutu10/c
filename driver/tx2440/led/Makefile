CROSS=arm-linux-

all: led

led:led.c
	$(CROSS)gcc -o led led.c
	$(CROSS)strip led
clean:
	@rm -vf led *.o *~
