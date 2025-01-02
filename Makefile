exec = clox.out                  # Executable name
src = $(wildcard src/*.c)         # Source files
obj = $(patsubst src/%.c, build/%.o, $(src))  # Object files
flags = -g                        # Compiler flags (for debugging)
inc_dir = -I./        # Include directory for header files (relative path)

# Rule to create the executable
$(exec): $(obj)
	$(CC) $(obj) $(flags) -o $(exec)        # Link object files into the executable

# Rule to create the object files
build/%.o: src/%.c | build
	gcc -c $(flags) $(inc_dir) $< -o $@     # Add include directory for header files

# Rule to create the build directory (if it doesn't exist)
build:
	mkdir -p build                         # Use -p to prevent errors if the directory exists

# Clean up the build artifacts
clean:
	rm -rf build                        # Remove the build directory and its contents
	rm -f $(exec)                        # Remove the executable

# Declare clean and build as phony targets (not actual files)
.PHONY: clean build
