CC=g++
CFLAGS= -c -std=c++11 -I../../shared/
LDFLAGS=-L../shared/ 
LIBS=
VPATH = src:../../shared
SOURCES=ProbFunc.cpp HistoPF.cpp ShiftPF.cpp Value.cpp NMFMatrix.cpp  driver.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=test_nmf

all: $(SOURCES) $(EXECUTABLE) 

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $^ -o $@ $(LIBS)

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:	
	rm -f *.o 
	touch *.cpp

