
TARGETS = dx21_rom_v1_5.bin \
          dx21_rom_v1_4.bin

all: $(TARGETS)

%.bin: %.asm
	dasm $? -f3 -o$@

clean:
	rm $(TARGETS)
