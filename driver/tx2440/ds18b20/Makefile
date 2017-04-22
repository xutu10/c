CROSS=arm-linux-

all: ds18b20

ds18b20:ds18b20.c
	$(CROSS)gcc -o ds18b20 ds18b20.c
	$(CROSS)strip ds18b20
clean:
	@rm -vf ds18b20 *.o *~
