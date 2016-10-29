CFLAGS = g++ -c -std=c++11 -Ofast -march=native -g
LFLAGS = g++ -std=c++11 -Ofast -march=native -g -o out

OBJS = state_vars.o state.o main.o

out : $(OBJS)
	$(LFLAGS) $(OBJS)

main.o : main.cpp state.h header.h state_vars.h
	$(CFLAGS) main.cpp

state.o : state.cpp state.h header.h state_vars.h
	$(CFLAGS) state.cpp

state_vars.o : state_vars.cpp state.h header.h state_vars.h
	$(CFLAGS) state_vars.cpp

clean :
	rm *.o
	rm out

run : out
	./out

debug : out
	gdb out