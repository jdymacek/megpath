CC=g++
CFLAGS=-O3 -c -fopenmp -std=c++11 -I../shared/
LDFLAGS=-L../shared/ -fopenmp
LIBS=-lpthread -lnmf
LIB_SOURCES=FlipThreadedMonteAnneal.cpp PhaseThreadedMonteAnneal.cpp ThreadedMonteAnneal.cpp
SOURCES=$(LIB_SOURCES) main.cpp
LIB_OBJECTS=$(LIB_SOURCES:.cpp=.o)
OBJECTS=$(SOURCES:.cpp=.o)
LIBRARY=libnmfthread.a
EXECUTABLE=threadednmf

all: $(SOURCES) $(LIBRARY) $(EXECUTABLE) 

$(LIBRARY): $(LIB_OBJECTS)
	ar rcs $@ $^

$(EXECUTABLE): $(OBJECTS)
	$(CC) -o $@ $^ $(LDFLAGS) $(LIBS)

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:	
	rm -f *.o 
	rm $(LIBRARY)
	rm $(EXECUTABLE)
	touch *.cpp

everything:
	$(MAKE) -C ../

cleanall:
	$(MAKE) clean -C ../
