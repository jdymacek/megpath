CC=g++
CFLAGS= -c -g -std=c++11 -I../../shared/
LDFLAGS=-L../../shared/ 
LIBS=-lnmf
VPATH = src:../../shared
SOURCES=driver.cpp
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

