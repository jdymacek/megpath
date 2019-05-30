CC=g++
CFLAGS=-c -g -std=c++11 -I../shared/ -I../threeMatrix
LDFLAGS=-L../../shared/ -L../../threeMatrix
LIBS=-lnmf -lpng
SOURCES=driver.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=test_range

all: $(SOURCES) $(EXECUTABLE) 

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $^ -o $@ $(LIBS)

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:	
	rm -f *.o 
	rm $(EXECUTABLE)
	touch *.cpp

everything:
	$(MAKE) -C ../

cleanall:
	$(MAKE) clean -C ../
