CC = gcc
CFLAGS = `pkg-config --cflags glib-2.0` -Wall -Wextra -fsanitize=address
LDLIBS = `pkg-config --libs glib-2.0`

all: main
	rm -rf *.o
main:
	gcc main.c sort.c list.c cellaut.c cell.c spellbook.c networking.c fsaut.c rbtree.c -Wall -Wextra -fsanitize=address -lm -lpthread
