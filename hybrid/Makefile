CC=mpic++
CFLAGS=-O3 -c -Wfatal-errors -std=c++11 -I../shared/ -I../threaded/ -I../distributed/
LDFLAGS=-L../shared/ -L../threaded/ -L../distributed/ 
LIBS=
VPATH = src:../shared src:../distributed src:../threaded /usr/lib64/openmpi/bin
SOURCES= FlipHybrid.cpp FlipThreadedMonteAnneal.cpp PhaseHybrid.cpp PhaseThreadedMonteAnneal.cpp PatternMatching.cpp FileUtil.cpp Distributed.cpp main.cpp PiecewisePF.cpp ParallelPatterns.cpp State.cpp ErrorFunction.cpp ErrorFunctionRow.cpp ErrorFunctionCol.cpp Analysis.cpp MonteAnneal.cpp ShiftPF.cpp Stopwatch.cpp UniformPF.cpp ArgFile.cpp CSVFile.cpp Value.cpp NMFMatrix.cpp Hybrid.cpp ProbFunc.cpp ThreadedMonteAnneal.cpp
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
