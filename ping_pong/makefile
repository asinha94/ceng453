CC=mpicc
LIBS=-lm
OUTPUTS := pingpong $(wildcard batch_sieve.sh.*)

build: pingpong.c 
	$(CC) -o pingpong pingpong.c $(LIBS)

clean:
	rm $(OUTPUTS)