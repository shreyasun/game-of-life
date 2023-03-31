CC = clang
CFLAGS = -Wall -Wpedantic -Werror -Wextra -g -gdwarf-4 
EXEC = life
OBJS = life.o universe.o

all: $(EXEC)

$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) -lncurses -o $(EXEC) $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f $(EXEC) $(OBJS)

format:
	clang-format -i -style=file *.[ch]

scan-build: clean
	scan-build --use-cc=$(CC) make

