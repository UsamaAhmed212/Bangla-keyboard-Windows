# Compiler and resource compiler
CC = gcc
WINDRES = windres

# Output executable name
TARGET = "Bangla Keyboard.exe"

# Directories
SRC_DIR = src
OBJ_DIR = obj
RC_DIR = rc

# Source files
SRC_FILES = $(SRC_DIR)/bangla_keyboard.c $(SRC_DIR)/splashScreen.c $(SRC_DIR)/keyboard.c \
            $(SRC_DIR)/systemTray.c $(SRC_DIR)/trayActions/programAutoStartup.c $(SRC_DIR)/trayActions/keyboardLayout.c
OBJ_FILES = $(OBJ_DIR)/resources.o $(OBJ_DIR)/version.res

# Resource scripts
RESOURCES = $(RC_DIR)/resources.rc
VERSION_RC = $(RC_DIR)/version.rc

# Compiler flags
CFLAGS = -lgdi32 -lole32 -mwindows

$(TARGET): $(SRC_FILES) $(OBJ_FILES)
	$(CC) $(SRC_FILES) $(OBJ_FILES) -o $(TARGET) $(CFLAGS)

# Compile resource files
$(OBJ_DIR)/resources.o: $(RESOURCES)
	$(WINDRES) $(RESOURCES) -o $(OBJ_DIR)/resources.o

$(OBJ_DIR)/version.res: $(VERSION_RC)
	$(WINDRES) $(VERSION_RC) -O coff -o $(OBJ_DIR)/version.res

# Ensure necessary directories exist
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)
	
# Default target
all: build

# Build the executable
build: $(TARGET)

# Run the executable
run: build
	./$(TARGET)

# Clean the build
clean:
	rm -f $(OBJ_FILES) $(TARGET)

