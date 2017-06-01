CC=g++
CFLAGS=-O3 -c -std=c++11 -I../shared/
LDFLAGS=-L../shared/ 
LIBS=
VPATH = src:../shared
SOURCES=main.cpp ShiftPF.cpp Stopwatch.cpp ProbFunc.cpp UniformPF.cpp HistoPF.cpp ArgFile.cpp CSVFile.cpp Value.cpp NMFMatrix.cpp Globals.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=connmf

all: $(SOURCES) $(EXECUTABLE) 

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $^ -o $@ $(LIBS)

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:	
	rm -f *.o 
	touch *.cpp

