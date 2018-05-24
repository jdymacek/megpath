CC=mpic++
CFLAGS=-O3 -c -Wfatal-errors -std=c++11 -I../shared/
LDFLAGS=-L../shared/ 
LIBS=
VPATH = src:../shared /usr/lib64/openmpi/bin
SOURCES=mpiTest.cpp 
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=testMpi

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
