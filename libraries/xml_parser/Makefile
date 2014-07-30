root = ../..

default: all

-include $(root)/configuration

all: all-recursive

all-here: libxmlparser.a

all-recursive: all-here

check: test/test
	./test/test > /dev/null

clean: clean-recursive

clean-here:
	$(RM) libxmlparser.a
	$(RM) source/xml_parser.o
	$(RM) test/test
	$(RM) test/test.o

clean-recursive: clean-here

libxmlparser.a: source/xml_parser.o
	ar rcs $@ $^

source/xml_parser.o: source/xml_parser.cpp include/xml_parser.h $(CXXFLAGS_configuration)
	$(CXX) $(CXXFLAGS) -Iinclude -c $< -o $@

test/test: test/test.o libxmlparser.a $(LDFLAGS_configuration)
	$(CXX) $(LDFLAGS) test/test.o libxmlparser.a -o $@

test/test.o: test/test.cpp include/xml_parser.h $(CXXFLAGS_configuration)
	$(CXX) $(CXXFLAGS) -Iinclude -c $< -o $@

.PHONY: default all all-here all-recursive check clean clean-here clean-recursive
