CC = gcc
RC = windres

# Object files
OBJS = obj/resources.o

# Output executable
EXEC = "Bangla Keyboard.exe"

# Default target
all: $(EXEC)

# Link object files and create executable
$(EXEC): $(OBJS)
	$(CC) $(OBJS) -o $(EXEC)

# Custom target to compile resources and build the executable
build: obj/resources.o
	$(RC) rc/resources.rc -o obj/resources.o && $(CC) src/bangla_keyboard.c obj/resources.o -o $(EXEC)

# Compile resource script to object file
obj/resources.o: rc/resources.rc include/resource.h
	$(RC) rc/resources.rc -o obj/resources.o

# Clean up object files and executable
clean:
	del /Q src\*.o obj\*.o $(EXEC)

# Run the build target
run: build
	./$(EXEC)
