CC=g++
CFLAGS= -c -std=c++11 -I../../shared/
LDFLAGS=-L../../shared/ 
LIBS=
VPATH = src:../../shared
SOURCES=UniformPF.cpp PiecewisePF.cpp CSVFile.cpp ProbFunc.cpp HistoPF.cpp ShiftPF.cpp NMFMatrix.cpp Value.cpp ArgFile.cpp State.cpp driver.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=test_pattern

all: $(SOURCES) $(EXECUTABLE) 

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $^ -o $@ $(LIBS)

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:	
	rm -f *.o 
	touch *.cpp

