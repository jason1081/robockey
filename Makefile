# --------------------------------------------------------
# Custom M2 Makefile
# written by: Jonathan Bohren & Jonathan Fiene
# updated: June 16, 2012
# --------------------------------------------------------

# --------------------------------------------------------
# if you write separate C files to include in main,
# add their .o targets to the CHILDREN line below
# (e.g. "CHILDREN = myfile.o")
#
# to include code supplied by maevarm, add a .o target
# tag to the parents line (e.g. "PARENTS = "m_bus.o")
# --------------------------------------------------------
MAIN       = vincent.o 
CHILDREN   = 
PARENTS    = m_bus.o m_port.o m_rf.o m_usb.o m_wii.o

# --------------------------------------------------------
# if you want to use one of our pre-compiled libraries,
# add it to the line below (e.g. "LIBRARIES = libsaast.a")
# --------------------------------------------------------

# --------------------------------------------------------
# you shouldn't change anything below here,
# unless you really know what you're doing
# --------------------------------------------------------
DEVICE     = atmega32u4
CLOCK      = 16000000

COMPILE = avr-gcc -Wall -Os -DF_CPU=$(CLOCK) -mmcu=$(DEVICE)

# symbolic targets: 
all:	vincent.hex

.c.o:
	$(COMPILE) -c $< -o $@

.S.o:
	$(COMPILE) -x assembler-with-cpp -c $< -o $@

.c.s:
	$(COMPILE) -S $< -o $@

install: flash 

flash: all
	dfu-programmer atmega32u4 erase
	dfu-programmer atmega32u4 flash vincent.hex

clean:
	rm -f vincent.hex vincent.elf $(MAIN) $(CHILDREN)

# file targets:
vincent.elf: $(MAIN) $(CHILDREN) $(PARENTS)
	$(COMPILE) -o vincent.elf $(MAIN) $(CHILDREN) $(PARENTS) $(LIBRARIES)-lm

vincent.hex: vincent.elf
	rm -f vincent.hex
	avr-objcopy -j .text -j .data -O ihex vincent.elf vincent.hex

# Targets for code debugging and analysis:
disasm:	vincent.elf
	avr-objdump -d vincent.elf

cpp:
	$(COMPILE) -E vincent.c
