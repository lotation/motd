# Compiler and Linker
CC = gcc

# The Target Binary Program
TARGET = motd

# Directories, Source, Includes, Objects and Binary
SRCDIR = src
INCDIR = include
BUILDDIR  = obj
TARGETDIR = bin

# Flags, Libraries and Includes
# SANITZE = -fsanitize=address,leak,alignment
# CFLAGS  = -Wall -Wextra -Werror -Wformat=2 -Wl,-dead_strip -Og $(SANITZE) -funroll-loops -fomit-frame-pointer -g3
CFLAGS  = -O3 -march=native -ffast-math
# LDFLAGS = $(SANITZE)
LIB     = -lalpm -lpacutils -lcurl 
INC     = -I $(INCDIR)

SOURCES = $(shell find $(SRCDIR) -type f -name *.c)
OBJECTS = $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.c=.o))

RM = rm
RMFLAGS = -rf

ifeq ($(PREFIX),)
    PREFIX := /usr/local
endif


# Defauilt Make
all: directories $(TARGET)

# Remake
remake: cleaner all

# Make the Directories
directories:
	@mkdir -p $(TARGETDIR)
	@mkdir -p $(BUILDDIR)

# Clean only Objecst
clean:
	@$(RM) $(RMFLAGS) $(BUILDDIR)

# Full Clean, Objects and Binaries
cleaner: clean
	@$(RM) $(RMFLAGS) $(TARGETDIR)

# Link
$(TARGET): $(OBJECTS)
	$(CC) $(LDFLAGS) -o $(TARGETDIR)/$(TARGET) $^ $(LIB)

# Compile
$(BUILDDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INC) -c -o $@ $<

install: $(TARGETDIR)/$(TARGET)
	install -Dm 755  $(TARGETDIR)/$(TARGET) $(PREFIX)/$(TARGETDIR)/$(TARGET)

uninstall:
	$(RM) $(RMFLAGS) $(PREFIX)/$(TARGETDIR)/$(TARGET)

# Non-File Targets
.PHONY: all remake clean cleaner
