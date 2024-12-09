CC = gcc
CFLAGS = -Wall -g

all: signal timer

signals: signal.c 
	$(CC) $(CFLAGS) signal.c -o signalab 

timer: timer.c 
	$(CC) $(CFLAGS) timer.c -o timer

clean:
	rm -f signalab timer
