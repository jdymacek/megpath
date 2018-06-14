CC=mpic++
CFLAGS=-O3 -c -Wfatal-errors -std=c++11 -I../shared/ -I../threaded/ -I../distributed/
LDFLAGS=-L../shared/ -L../threaded/ -L../distributed/ 
LIBS=-lnmf -lnmfthread
VPATH =src:../distributed src:../threaded /usr/lib64/openmpi/bin
SOURCES=PatternMatching.cpp Distributed.cpp main.cpp ParallelPatterns.cpp FuncThrow.cpp DistNaive.cpp ParallelFuncThrow.cpp 
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=hybridnmf

all: $(SOURCES) $(EXECUTABLE) 

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $^ -o $@ $(LIBS)

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:	
	rm -f *.o 
	touch *.cpp
	rm $(EXECUTABLE)

trials:
	g++ -std=c++11 readTrials.cpp ../shared/Value.cpp ../shared/ArgFile.cpp ../shared/FileUtil.cpp -o trials
