CC=g++
CFLAGS=-O3 -c -std=c++11 -I../shared/
LDFLAGS=-L../shared/ 
LIBS=
VPATH = src:../shared
SOURCES=gsea.cpp ArgFile.cpp CSVFile.cpp Value.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=gsea

all: $(SOURCES) $(EXECUTABLE) 

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $^ -o $@ $(LIBS)

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:	
	rm -f *.o 
	touch *.cpp
	rm -f $(EXECUTABLE)

