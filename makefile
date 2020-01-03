ifeq ($(DEBUG), true)
	CC = gcc -g
else
	CC = gcc
endif

all: control.o write.o semaphone.h 
	$(CC) -o control control.o 
	$(CC) -o write write.o

control.o: control.c semaphone.h 
	$(CC) -c control.c 

write.o: write.c semaphone.h
	$(CC) -c write.c 

clean:
	rm *.o; rm *.h.gch; rm story; rm control; rm write
