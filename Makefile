# Neural Link - GBA Visual Novel
# devkitARM Build System

# Toolchain
ARM7 := arm-none-eabi-
AS   := $(ARM7)as
CC   := $(ARM7)gcc
LD   := $(ARM7)gcc
OBJCOPY := $(ARM7)objcopy

# GBA specific flags
CFLAGS  := -O2 -mthumb -mthumb-interwork -Wall -Iinclude -Isrc/engine -Isrc/game -Isrc/script -Isrc/assets -Isrc/data
ASFLAGS := -mthumb
LDFLAGS := -mthumb -mthumb-interwork -T linker.ld -nostartfiles
LIBS    := -lgba

# Project name
PROJ    := neural_link
ROM     := build/$(PROJ).gba
ELF     := build/$(PROJ).elf
MAP     := build/$(PROJ).map

# Source directories
SRC_DIRS := src/main src/engine src/game src/script src/assets src/data

# Find all source files
C_SRCS   := $(foreach dir,$(SRC_DIRS),$(wildcard $(dir)/*.c))
S_SRCS   := $(foreach dir,$(SRC_DIRS),$(wildcard $(dir)/*.s))

# Object files
C_OBJS   := $(patsubst %.c,build/%.o,$(C_SRCS))
S_OBJS   := $(patsubst %.s,build/%.o,$(S_SRCS))
OBJS     := build/crt0.o $(C_OBJS) $(S_OBJS)

# Default target
all: $(ROM)

# Create build directories
$(C_OBJS): build/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c -o $@ $<

$(S_OBJS): build/%.o: %.s
	@mkdir -p $(dir $@)
	$(AS) $(ASFLAGS) -o $@ $<

build/crt0.o: src/main/crt0.s
	@mkdir -p $(dir $@)
	$(AS) $(ASFLAGS) -o $@ $<

# Link ELF
$(ELF): $(OBJS) linker.ld
	$(LD) $(LDFLAGS) -Wl,-Map,$(MAP) -o $@ $(OBJS) $(LIBS)

# Create GBA ROM
$(ROM): $(ELF)
	$(OBJCOPY) -O binary $< $@
	@echo "ROM built: $@"

# Clean
clean:
	rm -rf build

# Run in emulator (if mgba is available)
run: $(ROM)
	mgba-qt $(ROM) 2>/dev/null || mgba $(ROM) 2>/dev/null || echo "No GBA emulator found"

.PHONY: all clean run