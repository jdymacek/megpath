CC=mpic++
CFLAGS=-O3 -c -Wfatal-errors -std=c++11 -I../shared/
LDFLAGS=-L../shared/ 
LIBS=-lnmf -lpng
VPATH = /usr/lib64/openmpi/bin
LIB_SOURCES=PatternMatching.cpp Distributed.cpp FuncThrow.cpp DistNaive.cpp ParallelFuncThrow.cpp ParallelPatterns.cpp BlockParallel.cpp BlockThrow.cpp Centralized.cpp
SOURCES=$(LIB_SOURCES) main.cpp 
LIB_OBJECTS=$(LIB_SOURCES:.cpp=.o)
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=distnmf
LIBRARY=libnmfdist.a

all: $(SOURCES) $(LIBRARY) $(EXECUTABLE) 

$(LIBRARY): $(LIB_OBJECTS)
	ar rcs $@ $^

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $^ -o $@ $(LIBS)

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:	
	rm -f *.o 
	touch *.cpp
	rm $(LIBRARY)
	rm $(EXECUTABLE)

trials:
	g++ -std=c++11 readTrials.cpp ../shared/Value.cpp ../shared/ArgFile.cpp ../shared/FileUtil.cpp -o trials

everything:
	$(MAKE) -C ../

cleanall:
	$(MAKE) clean -C ../
