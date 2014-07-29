root = ../..

default: all

-include $(root)/configuration

all: all-recursive

all-here: source/xml_parser.o

all-recursive: all-here

check: test/test
	./test/test > /dev/null

clean: clean-recursive

clean-here: 
	$(RM) source/xml_parser.o
	$(RM) test/test
	$(RM) test/test.o

clean-recursive: clean-here

source/xml_parser.o: source/xml_parser.cpp include/xml_parser.h $(CXXFLAGS_configuration)
	$(CXX) $(CXXFLAGS) -Iinclude -c $< -o $@

test/test: test/test.o source/xml_parser.o $(LDFLAGS_configuration)
	$(CXX) $(LDFLAGS) test/test.o source/xml_parser.o -o $@

test/test.o: test/test.cpp include/xml_parser.h $(CXXFLAGS_configuration)
	$(CXX) $(CXXFLAGS) -Iinclude -c $< -o $@

.PHONY: default all all-here all-recursive check clean clean-here clean-recursive
