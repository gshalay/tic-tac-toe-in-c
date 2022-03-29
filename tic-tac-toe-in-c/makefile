# makefile.mk
# Student Name: Greg Shalay
# UoG: 1096689
# File Description: Contains the commands to compile code for A7 (ttt1.o, ttt2.o, show_node, count_nodes, alphatoe).

CC = gcc
CFLAGS = -std=c99 -Wall -pedantic

all: ttt1.o ttt2.o show_node count_nodes alphatoe

clean:
	rm *.o
	rm show_node
	rm count_nodes
	rm alphatoe

# EXECUTABLE FILE COMPILATION COMMANDS
show_node: show_node.o
	$(CC) $(CFLAGS) -o show_node ttt1.o ttt2.o show_node.o
	
count_nodes: count_nodes.o
	$(CC) $(CFLAGS) -o count_nodes ttt1.o ttt2.o count_nodes.o

alphatoe: alphatoe.o 
	$(CC) $(CFLAGS) -o alphatoe ttt1.o ttt2.o alphatoe.o


# OBJECT FILE COMPILATION COMMANDS
ttt1.o: ttt.h ttt1.c
	$(CC) $(CFLAGS) ttt1.c -o ttt1.o -c

ttt2.o: ttt.h ttt2.c
	$(CC) $(CFLAGS) ttt2.c -o ttt2.o -c

show_node.o: ttt.h show_node.c
	$(CC) $(CFLAGS) show_node.c -o show_node.o -c
	
count_nodes.o: ttt.h count_nodes.c
	$(CC) $(CFLAGS) count_nodes.c -o count_nodes.o -c
	
alphatoe.o: ttt.h alphatoe.c
	$(CC) $(CFLAGS) alphatoe.c -o alphatoe.o -c