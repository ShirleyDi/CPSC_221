mimic : mimic.o treap.o
	g++ -Wall -g mimic.o treap.o -o mimic

mimic.o : mimic.cc treap.h
	g++ -Wall -g -c mimic.cc

treap.o : treap.h treap.cc
	g++ -Wall -g -c treap.cc

clean :
	rm -f treap.o mimic.o mimic
