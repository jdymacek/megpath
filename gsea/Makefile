CC=mpic++
CFLAGS=-O3 -c -std=c++11 -I../shared/
LDFLAGS=-L../shared/ 
LIBS=
VPATH = src:../shared
SOURCES=FileUtil.cpp main.cpp DistributedGSEA.cpp GSEA.cpp Genome.cpp ArgFile.cpp CSVFile.cpp Value.cpp Pathway.cpp Gene.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=pathapp

all: $(SOURCES) $(EXECUTABLE) 

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $^ -o $@ $(LIBS)

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:	
	rm -f *.o 
	touch *.cpp
	rm -f $(EXECUTABLE)

