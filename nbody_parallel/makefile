CC=mpicc
LIBS=-lm
OUTPUTS := simulation $(wildcard batch_nbody.sh.*)

build: simulation.c 
	$(CC) -o simulation simulation.c $(LIBS)

clean:
	rm $(OUTPUTS)