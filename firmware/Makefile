BUILD_DIR = /home/ags/litex-lab/build_soc

# Importamos la configuración de LiteX
include $(BUILD_DIR)/software/include/generated/variables.mak
include $(SOC_DIRECTORY)/software/common.mak

# Forzamos los nombres si las variables de LiteX fallan
AS      = riscv64-unknown-elf-as
ASFLAGS = -march=rv32i -mabi=ilp32
CC      = riscv64-unknown-elf-gcc
OBJCOPY = riscv64-unknown-elf-objcopy

OBJECTS = boot.o main.o

all: firmware.bin

# 1. Creamos el BIN a partir del ELF
%.o: %.s
	$(AS) $(ASFLAGS) -o $@ $<
firmware.bin: firmware.elf
	$(OBJCOPY) -O binary $< $@

# 2. Creamos el ELF a partir de los objetos
# Usamos $(ld) pero asegurando que el output sea firmware.elf
firmware.elf: $(OBJECTS)
	$(CC) $(LDFLAGS) \
		-T linker.ld \
		-T $(BUILD_DIR)/software/include/generated/output_format.ld \
		-T $(BUILD_DIR)/software/include/generated/regions.ld \
		-o $@ $(OBJECTS) $(PACKAGES:%=-L$(BUILD_DIR)/software/%) $(LIBS:lib%=-l%)

# 3. Compilamos el C
%.o: %.c
	$(compile)

clean:
	rm -f *.o *.elf *.bin *.d

.PHONY: all clean