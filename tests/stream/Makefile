vpath %.c ../../src/stream
vpath %.c ../../src/stream/interface

OBJECTS  = stream.o read.o write.o reader.o writer.o

%.o: %.c
	gcc -Wall -ggdb -O0 -fprofile-arcs -ftest-coverage -I ../../include -c $< -o $@

.PHONY: all clean check distclean check-build
all: check-build
check: check-build
check-build: $(OBJECTS)
distclean: clean
clean:
	rm -rf $(OBJECTS)
