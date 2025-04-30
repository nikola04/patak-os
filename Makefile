# Compiler
ASM :=	 	nasm
CC := 		x86_64-elf-gcc
LD :=		x86_64-elf-ld

# Config
LCONFIG :=	linker.ld

# Emulators
QEMU := 	qemu-system-x86_64

# Directories
B := 		build
BI :=		bin
BL := 		bootloader
K := 		kernel

# Flags
CFLAGS := 	-g -m64 -ffreestanding -nostdlib -nostartfiles -nodefaultlibs -Wall -O0
LDFLAGS :=	-T $(LCONFIG) --oformat binary

# Files
K_BIN :=	$(BI)/kernel.bin
B_BIN :=	$(BI)/bootloader.bin
OS_BIN :=	$(BI)/os.bin

# OBJS
OBJS = 		\
			$B/kernel.o\
			$B/spinlock.o\
			$B/console.o\
			$B/string.o\

# Target
TARGET :=	simpleos.img

all: $(TARGET)

# Build image
$(TARGET): $(K_BIN) $(B_BIN)
	dd if=$(B_BIN) of=$(OS_BIN) bs=512 seek=0 conv=notrunc
	dd if=$(K_BIN) of=$(OS_BIN) bs=512 seek=1 conv=notrunc
	truncate -s 1474560 $(OS_BIN)
	cp $(OS_BIN) $(TARGET)

# Link kernel objects into bin
$(K_BIN): $(OBJS) $(B)/kernel.asm.o
	@mkdir -p $(BI)
	$(LD) $(LDFLAGS) -o $@ $^

# Compile c to object
$(B)/%.o: $(K)/%.c
	@mkdir -p $(B)
	$(CC) $(CFLAGS) -std=gnu99 -c $< -o $@

# Kernel asm
$(B)/kernel.asm.o: $(K)/kernel.asm
	@mkdir -p $(B)
	$(ASM) -f elf64 -g $< -o $@

# Bootloader asm to bin
$(B_BIN): $(BL)/main.asm
	@mkdir -p $(BI)
	$(ASM) -f bin $< -o $@

.PHONY: qemu clean
# Running with QEMU
qemu: $(TARGET)
	$(QEMU) -drive format=raw,file=$(TARGET)

# Clean build dir
clean:
	rm -rf $(B)/*.o $(B)
	rm -rf $(BI)/*.bin $(BI)
