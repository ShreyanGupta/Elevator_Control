CFLAGS = g++ -c -std=c++11 -Ofast -march=native -g
LFLAGS = g++ -std=c++11 -Ofast -march=native -g -o out

OBJS = main.o

out : $(OBJS)
	$(LFLAGS) $(OBJS)

main.o : main.cpp header.h
	$(CFLAGS) main.cpp

clean :
	rm *.o
	rm out

run : out
	./out

debug : out
	gdb out