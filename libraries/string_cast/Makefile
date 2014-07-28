root = ../..

default: all

-include $(root)/configuration

all: all-recursive

all-here: source/string_cast.o

all-recursive: all-here

check: test/test
	./test/test > /dev/null

clean: clean-recursive

clean-here: 
	$(RM) source/string_cast.o
	$(RM) test/test
	$(RM) test/test.o

clean-recursive: clean-here

source/string_cast.o: source/string_cast.cpp include/string_cast.h $(CXXFLAGS_configuration)
	$(CXX) $(CXXFLAGS) -Iinclude -c $< -o $@

test/test: test/test.o source/string_cast.o $(LDFLAGS_configuration)
	$(CXX) $(LDFLAGS) test/test.o source/string_cast.o -o $@

test/test.o: test/test.cpp include/string_cast.h $(CXXFLAGS_configuration)
	$(CXX) $(CXXFLAGS) -Iinclude -c $< -o $@

.PHONY: default all all-here all-recursive check clean clean-here clean-recursive
