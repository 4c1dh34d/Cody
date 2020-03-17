CC = gcc
CFLAGS = -Wall -Wno-deprecated-declarations -g
all:
	$(CC) $(CFLAGS) `pkg-config --cflags --libs gtk+-3.0 gtksourceview-4` main.c
