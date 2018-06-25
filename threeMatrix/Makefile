CC=g++
CFLAGS=-O3 -c -lpthread -std=c++11 -Wfatal-errors -Werror -I ../shared
LDFLAGS= -L ../shared/ 
LIBS=-lnmf
SOURCES= main.cpp ThreeAnalysis.cpp ThreeState.cpp ThreeMonteAnneal.cpp ThreeErrorFunction.cpp
OBJECTS=$(SOURCES:.cpp=.o)
LIBRARY=lib3nmf.a
EXECUTABLE=threenmf

all: $(SOURCES) $(OBJECTS) $(EXECUTABLE) $(LIBRARY)

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $^ -o $@ $(LIBS)

$(LIBRARY): $(OBJECTS)
	ar rcs $@ $^

clean:	
	rm -f *.o 
	touch *.cpp

everything:
	$(MAKE) -C ../

cleanall:
	$(MAKE) clean -C ../
