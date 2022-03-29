# Compiler and Linker
CC = gcc

# The Target Binary Program
TARGET = motd

# Directories, Source, Includes, Objects and Binary
SRCDIR = src
INCDIR = lib
BUILDDIR  = obj
TARGETDIR = bin

# Flags, Libraries and Includes
CFLAGS = -Ofast
LIB    = -lalpm -lcurl
INC    = -I $(INCDIR)

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
	$(CC) -o $(TARGETDIR)/$(TARGET) $^ $(LIB)

# Compile
$(BUILDDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INC) -c -o $@ $<

install: $(TARGETDIR)/$(TARGET)
	install -m 755 $(TARGETDIR)/$(TARGET) $(PREFIX)/$(TARGETDIR)/$(TARGET)

uninstall:
	$(RM) $(RMFLAGS) $(PREFIX)/$(TARGETDIR)/$(TARGET)

# Non-File Targets
.PHONY: all remake clean cleaner
