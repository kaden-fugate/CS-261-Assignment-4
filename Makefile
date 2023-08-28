CC=gcc --std=c99 -g

all: test_pq dijkstra

test_pq: test_pq.c pq.o dynarray.o
	$(CC) test_pq.c pq.o dynarray.o -g -o test_pq

dijkstra: dijkstra.c pq.o dynarray.o
	$(CC) dijkstra.c pq.o dynarray.o -g -o dijkstra

dynarray.o: dynarray.c dynarray.h
	$(CC) -c dynarray.c

pq.o: pq.c pq.h
	$(CC) -c pq.c

clean:
	rm -f *.o test_pq dijkstra
	rm -rf *.dSYM/
