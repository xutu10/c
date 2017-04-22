CROSS=arm-linux-

all: gps

gps: gps.c
	$(CROSS)gcc -o gps gps.c
	$(CROSS)strip gps
clean:
	@rm -vf gps *.o *~
