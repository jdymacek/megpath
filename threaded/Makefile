CC=g++
CFLAGS=-O3 -c -fopenmp -std=c++11 -I../shared/
LDFLAGS=-L../shared/ -fopenmp
LIBS=-lpthread -lnmf
SOURCES=Threaded.cpp FlipThreadedMonteAnneal.cpp PhaseThreaded.cpp PhaseThreadedMonteAnneal.cpp ThreadedMonteAnneal.cpp main.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=threadednmf

all: $(SOURCES) $(EXECUTABLE) 

$(EXECUTABLE): $(OBJECTS)
	$(CC) -o $@ $^ $(LDFLAGS) $(LIBS)

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:	
	rm -f *.o 
	rm $(EXECUTABLE)
	touch *.cpp

