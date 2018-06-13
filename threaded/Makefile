CC=g++
CFLAGS=-O3 -c -fopenmp -std=c++11 -I../shared/
LDFLAGS=-L../shared/ -fopenmp
LIBS=-lnmf -lpthread
SOURCES=FlipThreadedMonteAnneal.cpp PhaseThreaded.cpp PhaseThreadedMonteAnneal.cpp ThreadedMonteAnneal.cpp main.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=threadednmf

all: $(SOURCES) $(EXECUTABLE) 

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $^ -o $@ $(LIBS)

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:	
	rm -f *.o 
	rm $(EXECUTABLE)
	touch *.cpp

