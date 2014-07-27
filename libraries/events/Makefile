root = ../..

default: all

-include $(root)/configuration

all: all-recursive

all-here: source/connection.o

all-recursive: all-here

check: test/test
	./test/test > /dev/null

clean: clean-recursive

clean-here: 
	$(RM) source/connection.o
	$(RM) test/test
	$(RM) test/test.o

clean-recursive: clean-here

source/connection.o: source/connection.cpp include/connection.h $(CXXFLAGS_configuration)
	$(CXX) $(CXXFLAGS) -Iinclude -c $< -o $@

test/test: source/connection.o test/test.o $(LDFLAGS_configuration)
	$(CXX) $(LDFLAGS) source/connection.o test/test.o -o $@

test/test.o: test/test.cpp include/connection.h include/event.h $(CXXFLAGS_configuration)
	$(CXX) $(CXXFLAGS) -Iinclude -c $< -o $@

.PHONY: default all all-here all-recursive check clean clean-here clean-recursive
