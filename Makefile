PRG            = eos_prog
OBJ            = ${PRG}.o
PROGRAMMER     = arduino
PORT           = /dev/ttyUSB0
MCU_TARGET     = atmega328p 
AVRDUDE_TARGET = atmega328p
OPTIMIZE       = -O3
DEFS           =
LIBS           =
BAUDRATE       = 115200

HZ	           = 16000000

CC             = avr-gcc

SRC_DIR = src
BUILD_DIR = build

override CFLAGS        = -std=c99 -DF_CPU=$(HZ) -Wall $(OPTIMIZE) -mmcu=$(MCU_TARGET) $(DEFS) -I$(realpath $(SRC_DIR)/eos)
override LDFLAGS       = -Wl,-Map,$(BUILD_DIR)/$(PRG).map

OBJCOPY        = avr-objcopy
OBJDUMP        = avr-objdump
SIZE           = avr-size
AVRDUDE        = avrdude
SCREEN         = screen
MINITERM       = pyserial-miniterm

BOOT_PAD       = $(BUILD_DIR)/.bootloader-pad

SRCS := $(shell find $(SRC_DIR) -name '*.c')
OBJS := $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o, $(SRCS))

.PHONY: clean always all install text
always: 
	mkdir -p build
clean:
	rm -rf *.o $(PRG).elf *.eps *.png *.pdf *.bak *.hex *.bin *.srec
	rm -rf *.lst *.map $(BOOT_PAD) $(EXTRA_CLEAN_FILES)
	rm -rf $(BUILD_DIR) $(PRG)

all: always $(PRG) lst text

$(PRG): $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $(PRG) $(OBJS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

lst:  $(BUILD_DIR)/$(PRG).lst

$(BUILD_DIR)/$(PRG).lst: $(PRG)
	$(OBJDUMP) -h -S $< > $@

text: hex bin srec

hex:  $(BUILD_DIR)/$(PRG).hex
bin:  $(BUILD_DIR)/$(PRG).bin
srec: $(BUILD_DIR)/$(PRG).srec

$(BUILD_DIR)/$(PRG).hex: $(PRG) $(BOOT_PAD)
	$(OBJCOPY) -j .text -j .data -O ihex $< $@
	$(OBJCOPY) --add-section .bootloader=$(BOOT_PAD) $< $(BUILD_DIR)/.padded-$<
	$(SIZE) --mcu=${MCU_TARGET} --format=avr $(BUILD_DIR)/.padded-$<

$(BUILD_DIR)/$(PRG).srec: $(PRG)
	$(OBJCOPY) -j .text -j .data -O srec $< $@

$(BUILD_DIR)/$(PRG).bin: $(PRG)
	$(OBJCOPY) -j .text -j .data -O binary $< $@

install:  $(BUILD_DIR)/$(PRG).hex
	$(AVRDUDE) -p $(AVRDUDE_TARGET) -c $(PROGRAMMER) -P $(PORT) \
        -b $(BAUDRATE) -v -U flash:w:$(BUILD_DIR)/$(PRG).hex 

screen:
	$(SCREEN) $(PORT)

miniterm:
	$(MINITERM) $(PORT)

$(BOOT_PAD):
	dd if=/dev/zero of=$(BOOT_PAD) bs=512 count=1


