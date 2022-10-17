# Main build process
# Creates the executable
build: main
main: src/obj/bfc.o
	gcc -o bfc src/obj/bfc.o
	mkdir -p bin
	mv bfc bin

# Object files
src/obj/bfc.o: src/bfc.c
	gcc -c src/bfc.c
	mkdir -p src/obj
	mv bfc.o src/obj

# Clean out the binary and object folders
clean:
	rm -r bin
	rm -r src/obj
