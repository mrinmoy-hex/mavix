CC = gcc
CFLAGS = -g -Wall -Wextra   # Enable debugging and warnings
LDFLAGS =                   # Add linker flags if needed

EXEC = mavix           # Executable name
SRC = $(wildcard src/*.c)   # Source files
OBJ = $(patsubst src/%.c, build/%.o, $(SRC))  # Object files
INC_DIR = -I./             # Include directory

# Rule to create the executable
$(EXEC): $(OBJ) | build
	$(CC) $(OBJ) $(LDFLAGS) -o $(EXEC)        

# Rule to create object files
build/%.o: src/%.c | build
	$(CC) -c $(CFLAGS) $(INC_DIR) $< -o $@     

# Rule to create build directory if it doesn’t exist
build:
	mkdir -p build


install: $(EXEC)
	mkdir -p dist                  # Ensure dist directory exists
	cp $(EXEC) dist/mavix          # Copy the executable to dist
	cp dist/mavix /usr/local/bin/mavix  # Install it to /usr/local/bin


# Clean up build artifacts
clean:
	rm -rf build dist/mavix /usr/local/bin/mavix

# Declare clean and build as phony targets
.PHONY: clean build
