CC=mpicc
CFLAGS=-I
DEPS=ring.o dotProduct.o hello.o

all: $(DEPS)

%.o: %.C
	$(CC) $< -o $@ $(FLAGS) -c

clean:
	rm -rf *.o main
