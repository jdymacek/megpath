CC=mpic++
CFLAGS=-O3 -c -Wfatal-errors -std=c++11 -I../shared/
LDFLAGS=-L../shared/ 
LIBS=
VPATH = src:../shared /usr/lib64/openmpi/bin
SOURCES=PatternMatching.cpp FileUtil.cpp Distributed.cpp main.cpp PiecewisePF.cpp ParallelPatterns.cpp FuncThrow.cpp DistNaive.cpp State.cpp ErrorFunction.cpp ErrorFunctionRow.cpp ErrorFunctionCol.cpp Analysis.cpp MonteAnneal.cpp ShiftPF.cpp Stopwatch.cpp ProbFunc.cpp UniformPF.cpp ArgFile.cpp CSVFile.cpp Value.cpp NMFMatrix.cpp ParallelFuncThrow.cpp 
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
	rm $(EXECUTABLE)

trials:
	g++ -std=c++11 readTrials.cpp ../shared/Value.cpp ../shared/ArgFile.cpp ../shared/FileUtil.cpp -o trials
