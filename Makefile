FLAGS = -Wall -Wextra
CC = gcc
OBJS = main.o weather.o


all: final

# target: dependencies
#    commands

final: $(OBJS)
	$(CC) $(FLAGS) $(OBJS) -o final 

main.o: main.c
	$(CC) $(FLAGS) -c main.c -o main.o

weather.o: weather.c
	$(CC) $(FLAGS) -c weather.c -o weather.o



clean:
	rm $(OBJS) final