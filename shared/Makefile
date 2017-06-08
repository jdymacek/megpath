CC=g++
CFLAGS=-c -std=c++11 -Wfatal-errors -Werror
LDFLAGS= 
LIBS=
SOURCES=MonteAnneal.cpp ErrorFunctionCol.cpp ErrorFunctionRow.cpp ErrorFunction.cpp Analysis.cpp State.cpp HybridPF.cpp NMFMatrix.cpp FileUtil.cpp ArgFile.cpp FixedPF.cpp NormalPF.cpp ShiftPF.cpp Value.cpp CSVFile.cpp HistoPF.cpp ProbFunc.cpp UniformPF.cpp
OBJECTS=$(SOURCES:.cpp=.o)

all: $(SOURCES) $(OBJECTS)

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:	
	rm -f *.o 
	touch *.cpp

