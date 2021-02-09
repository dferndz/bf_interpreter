CC=gcc
CF=-std=c11
OBJS=bf.o interpreter.o
OUTPUT=bf

all: $(OBJS)
	$(CC) $(CF) $(OBJS) -o $(OUTPUT)

%.o: %.c
	$(CC) $(CF) -c $< -o $@

clean:
	rm -f *.o
	rm $(OUTPUT)

install: all
	sudo cp bf /usr/local/bin/