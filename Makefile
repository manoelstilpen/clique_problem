COMPILER = g++ --std=c++11 -Wall -g -O3
LIBRARIES_BOOST = -lboost_system -lboost_filesystem -lboost_program_options
EXEC_PROG = clique
BINARIES = $(EXEC_PROG)

SOURCES := $(shell find -name '*.cpp')

OBJECTS = $(SOURCES:.cpp=.o)

%.o: %.cpp
	$(COMPILER) -c $(LIBRARIES_BOOST) -o $@ $< -w

all: $(EXEC_PROG)
	@echo Build Completed

$(EXEC_PROG): $(OBJECTS)
	$(COMPILER) -o $(EXEC_PROG) $(OBJECTS) $(LIBRARIES_BOOST)

clean:
	rm *.o $(BINARIES)