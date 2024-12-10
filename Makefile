SOURCES = main.c list.c
OBJS = $(SOURCES:.c=.o)
TARGET = list
CC = gcc
OPTS = -Wall

$(TARGET): $(OBJS)
	$(CC) -o $(TARGET) $(OBJS) $(LIBS)

%.o: %.c
	$(CC) $(OPTS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)
