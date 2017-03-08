# My first make file. Let's see how it goes
#http://www.linuxquestions.org/questions/programming-9/shell-script-and-makefile-126703/
# To burn as normal user: http://mightyohm.com/blog/2010/03/run-avrdude-without-root-privs-in-ubuntu/
#https://www.cs.swarthmore.edu/~newhall/unixhelp/howto_makefiles.html

TARGET = fencebot
MCU    = atmega328p
F_CPU  = 1000000
# Figure out how to do this
# OTHER_SOURCE = shift

all: fencebot burn clean
  # How to make this target also burn and clean?

fencebot: $(TARGET).c shift.elf stepper.elf
	avr-gcc -std=gnu99 -DF_CPU=$(F_CPU)UL -O -mmcu=atmega328p -o $(TARGET).elf $(TARGET).c shift.c stepper.c
	# gnu99 for for loop initial declarations
	# -O to optimize
	
	avr-objcopy -j .text -j .data -O ihex $(TARGET).elf $(TARGET).hex
	
shift.elf:
	avr-gcc -std=gnu99 -DF_CPU=$(F_CPU)UL -O -c -mmcu=atmega328p -o shift.elf shift.c


stepper.elf:
	avr-gcc -std=gnu99 -DF_CPU=$(F_CPU)UL -O -c -mmcu=atmega328p -o stepper.elf stepper.c
	

burn: $(TARGET).hex
	avrdude -p $(MCU) -c avrispmkII -e -U flash:w:$(TARGET).hex
  
clean:
	rm -f *.hex *.elf
