# Compiler and resource compiler
CC = gcc
WINDRES = windres

# Output executable name
TARGET = "Bangla Keyboard.exe"

# Directories
SRC_DIR = src
OBJ_DIR = obj
RC_DIR = rc
RES_DIR = resources

# Source files
SRC_FILES = $(SRC_DIR)/bangla_keyboard.c $(SRC_DIR)/splashScreen.c $(SRC_DIR)/systemTray.c
OBJ_FILES = $(OBJ_DIR)/resources.o

# Resource script
RESOURCES = $(RC_DIR)/resources.rc

# Compiler flags
CFLAGS = -mwindows

# Default target
all: build

# Build the executable
build: $(TARGET)

$(TARGET): $(SRC_FILES) $(OBJ_FILES)
	$(CC) $(SRC_FILES) $(OBJ_FILES) -o $(TARGET) $(CFLAGS)

# Run the executable
run: build
	./$(TARGET)

# Compile resource file
$(OBJ_DIR)/resources.o: $(RESOURCES)
	$(WINDRES) $(RESOURCES) -o $(OBJ_DIR)/resources.o

# Clean the build
clean:
	rm -f $(OBJ_FILES) $(TARGET)

