make: BTrees.o
	g++ BTrees.o -o make

BTrees.o: BTrees.cpp
	g++ -c BTrees.cpp

clean: 
	rm *.o
	rm make

debug: make
	gdb ./make

run: make
	./make

