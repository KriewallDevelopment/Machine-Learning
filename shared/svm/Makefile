CC= g++
CFLAGS= -c -g -Wall -I include/ --std=c++0x -W -Wall -Wno-sign-compare -O2 -s -pipe -mmmx -msse -msse2 -msse3

SOURCES= $(shell find * -type f -name '*.cpp')
OBJECTS= $(SOURCES:.cpp=.o)
OUTPUT= analyzer

all: $(SOURCES) $(OUTPUT)
	@echo "Done."

$(OUTPUT): $(OBJECTS)
	@$(CC) $(OBJECTS) -o $@ $(LIBS)
	@echo "LD $@"

.cpp.o:
	@$(CC) $(CFLAGS) $< -o $@
	@echo "CC $<"
	

clean:
	@-rm -rf $(OBJECTS) $(OUTPUT)

love:
	@echo "Not war?"
