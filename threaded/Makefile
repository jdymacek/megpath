CC=g++
CFLAGS=-O3 -c -std=c++11 -I../shared/
LDFLAGS=-L../shared/ -lpthread 
LIBS=
VPATH = src:../shared
SOURCES= PhaseThreaded.cpp PhaseThreadedMonteAnneal.cpp ThreadedMonteAnneal.cpp FileUtil.cpp PiecewisePF.cpp MonteAnneal.cpp State.cpp main.cpp ShiftPF.cpp Stopwatch.cpp ProbFunc.cpp UniformPF.cpp HistoPF.cpp ArgFile.cpp CSVFile.cpp Value.cpp NMFMatrix.cpp Analysis.cpp ErrorFunction.cpp ErrorFunctionRow.cpp ErrorFunctionCol.cpp Threaded.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=threadednmf

all: $(SOURCES) $(EXECUTABLE) 

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $^ -o $@ $(LIBS)

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:	
	rm -f *.o 
	rm $(EXECUTABLE)
	touch *.cpp

