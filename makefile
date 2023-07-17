# Make file for extra credit assignment, Virtual Farm

CC	= g++ 
CCFLAGS = -Wall -Wextra -pedantic -std=c++11 -g

OBJS	= main.o debug.o farm.o farm_object.o
DEPS	= farm.h farm_object.h

all: main

main: $(OBJS)
	@echo "Compiling Virtual Farm..."
	$(CC) $(CCFLAGS) -o virt_farm $(OBJS)
	@echo "Done! Run virt_farm"

main.o:	main.cpp $(DEPS)
	$(CC) $(CCFLAGS) -c main.cpp

farm_object.o:	farm_object.cpp $(DEPS)
	$(CC) $(CCFLAGS) -c farm_object.cpp

farm.o:	farm.cpp $(DEPS)
	$(CC) $(CCFLAGS) -c farm.cpp
	
debug.o: debug.cpp $(DEPS) debug.h
	$(CC) $(CCFLAGS) -c debug.cpp
	
clean:
	rm $(OBJS) virt_farm
	
