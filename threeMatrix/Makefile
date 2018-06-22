CC=g++
CFLAGS=-O3 -c -lpthread -std=c++11 -Wfatal-errors -Werror -I ../shared
LDFLAGS= -L ../shared/ 
LIBS=-lnmf
SOURCES= ThreeState.cpp ThreeMonteAnneal.cpp ThreeErrorFunction.cpp
OBJECTS=$(SOURCES:.cpp=.o)
LIBRARY=lib3nmf.a

all: $(SOURCES) $(OBJECTS) $(LIBRARY)

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

$(LIBRARY): $(OBJECTS)
	ar rcs $@ $^

clean:	
	rm -f *.o 
	touch *.cpp

everything:
	$(MAKE) -C ../

cleanall:
	$(MAKE) clean -C ../
