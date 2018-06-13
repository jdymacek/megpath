CC=g++
CFLAGS=-c -std=c++11 -Wfatal-errors -Werror
LDFLAGS= 
LIBS=
SOURCES=PiecewisePF.cpp MonteAnneal.cpp ErrorFunctionCol.cpp ErrorFunctionRow.cpp ErrorFunction.cpp Analysis.cpp State.cpp HybridPF.cpp NMFMatrix.cpp FileUtil.cpp ArgFile.cpp FixedPF.cpp NormalPF.cpp ShiftPF.cpp Value.cpp CSVFile.cpp HistoPF.cpp ProbFunc.cpp UniformPF.cpp
OBJECTS=$(SOURCES:.cpp=.o)
LIBRARY=libnmf.a

all: $(SOURCES) $(OBJECTS) $(LIBRARY)

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

$(LIBRARY): $(OBJECTS)
	ar rcs $@ $^

clean:	
	rm -f *.o 
	touch *.cpp

