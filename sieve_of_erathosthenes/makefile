CC=mpicc
LIBS=-lm
OUTPUTS := sieve $(wildcard batch_sieve.sh.*)

build: sieve.c 
	$(CC) -o sieve sieve.c $(LIBS)

clean:
	rm $(OUTPUTS)