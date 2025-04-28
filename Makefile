# Compiler
ASM=nasm

# Directories
SRC_DIR=bootloader
BUILD_DIR=build

# Emulators
QEMU = qemu-system-i386

# Build image
simpleos.img: $(BUILD_DIR)/main.bin
	cp $(BUILD_DIR)/main.bin $(BUILD_DIR)/simpleos.img
	truncate -s 1440k simpleos.img

# Compile asm to bin
$(BUILD_DIR)/main.bin: $(SRC_DIR)/main.asm
	@mkdir -p $(BUILD_DIR)
	$(ASM) $(SRC_DIR)/main.asm -f bin -o $(BUILD_DIR)/main.bin

# Running with QEMU
qemu:
	$(QEMU) $(BUILD_DIR)/simpleos.img

# Clean build dir
clean:
	rm -rf $(BUILD_DIR)/*.bin $(BUILD_DIR)/*.img $(BUILD_DIR)
