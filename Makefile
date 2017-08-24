COMPILER = g++ -std=c++11 -Wall -g
EXEC_PROG = clique
BINARIES = $(EXEC_PROG)

SOURCES := $(shell find -name '*.cpp')

OBJECTS = $(SOURCES:.cpp=.o)

%.o: %.c
	$(COMPILER) -c -o $@ $< -w

all: $(EXEC_PROG)
	@echo Build Completed

$(EXEC_PROG): $(OBJECTS)
	$(COMPILER) -o $(EXEC_PROG) $(OBJECTS)

clean:
	rm *.o $(BINARIES)