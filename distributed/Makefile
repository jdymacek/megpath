CC=mpic++
CFLAGS=-O3 -c -std=c++11 -I../shared/
LDFLAGS=-L../shared/ 
LIBS=
VPATH = src:../shared
SOURCES=FuncThrow.cpp Globals.cpp DistNaive.cpp State.cpp ErrorFunction.cpp ErrorFunctionRow.cpp ErrorFunctionCol.cpp Analysis.cpp MonteAnneal.cpp main.cpp ShiftPF.cpp Stopwatch.cpp ProbFunc.cpp UniformPF.cpp HistoPF.cpp ArgFile.cpp CSVFile.cpp Value.cpp NMFMatrix.cpp 
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=distnmf

all: $(SOURCES) $(EXECUTABLE) 

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $^ -o $@ $(LIBS)

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:	
	rm -f *.o 
	touch *.cpp

