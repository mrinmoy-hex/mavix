exec = clox.out                  # Executable name
src = $(wildcard src/*.c)         # Source files
obj = $(patsubst src/%.c, build/%.o, $(src))  # Object files
flags = -g                        # Compiler flags (for debugging)
inc_dir = -I./        # Include directory for header files (relative path)

# Rule to create the executable
$(exec): $(obj)
# Link object files into the executable
	$(CC) $(obj) $(flags) -o $(exec)        

# Rule to create the object files
build/%.o: src/%.c | build
# Add include directory for header files
	gcc -c $(flags) $(inc_dir) $< -o $@     

# Rule to create the build directory (if it doesn't exist)
build:
# Use -p to prevent errors if the directory exists
	mkdir -p build                         

# Clean up the build artifacts
clean:
# Remove the build directory and its contents
	rm -rf build                        
	rm -f $(exec)                        

# Declare clean and build as phony targets (not actual files)
.PHONY: clean build
