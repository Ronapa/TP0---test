CCFLAGS= -Wall -g -pedantic -ansi
CC= g++ 

all: TP0 clean

TP0: main.o sensor.o system.o query.o cmdline.o utilities.o
	$(CC) $(CCFLAGS) -o TP0 main.o sensor.o query.o system.o cmdline.o utilities.o

main.o: main.cc 
	$(CC) $(CCFLAGS) -c main.cc -o main.o 

cmdline.o: cmdline.cc cmdline.h
	$(CC) $(CCFLAGS) -c cmdline.cc -o cmdline.o 

query.o: query.cc
	$(CC) $(CCFLAGS) -c query.cc -o query.o	

system.o: system.cc
	$(CC) $(CCFLAGS) -c system.cc -o system.o	

sensor.o: sensor.cc
	$(CC) $(CCFLAGS) -c sensor.cc -o sensor.o	

utilities.o: utilities.cc
	$(CC) $(CCFLAGS) -c utilities.cc -o utilities.o	

clean:
	$(RM) *.o 