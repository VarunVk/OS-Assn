all: graphexec

graphexec: votecounter.c makeargv.h open_read.o
	gcc votecounter.c open_read.o -o votecounter

clean:
	rm votecounter
	rm *.out

cleanOut:
	rm *.out
