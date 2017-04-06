CROSS=arm-linux-

all: adc

adc:adc.c
	$(CROSS)gcc -o adc adc.c
	$(CROSS)strip adc
clean:
	@rm -vf adc *.o *~
