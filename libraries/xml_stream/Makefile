root = ../..

default: all

-include $(root)/configuration

all: all-recursive

all-here: source/xml_stream.o

all-recursive: all-here

check: test/test
	./test/test

clean: clean-recursive

clean-here: 
	$(RM) source/xml_stream.o
	$(RM) test/test

clean-recursive: clean-here

source/xml_stream.o: source/xml_stream.cpp include/xml_stream.h $(CXXFLAGS_configuration)
	$(CXX) -std=c++11 $(CXXFLAGS) -Iinclude -c $< -o $@

test/test: test/test.o source/xml_stream.o $(LDFLAGS_configuration)
	$(CXX) $(LDFLAGS) test/test.o source/xml_stream.o -o $@

test/test.o: test/test.cpp include/xml_stream.h $(CXXFLAGS_configuration)
	$(CXX) -std=c++11 $(CXXFLAGS) -Iinclude -c $< -o $@

.PHONY: default all all-here all-recursive check clean clean-here clean-recursive
