root = ../..

default: all

-include $(root)/configuration

all: all-recursive

all-here: libxmlstream.a

all-recursive: all-here

check: test/test
	./test/test > /dev/null

clean: clean-recursive

clean-here: 
	$(RM) libxmlstream.a
	$(RM) source/xml_stream.o
	$(RM) test/test
	$(RM) test/test.o

clean-recursive: clean-here

libxmlstream.a: source/xml_stream.o
	ar -rcsv $@ $^

source/xml_stream.o: source/xml_stream.cpp include/xml_stream.h $(CXXFLAGS_configuration)
	$(CXX) $(CXXFLAGS) -Iinclude -c $< -o $@

test/test: test/test.o libxmlstream.a $(LDFLAGS_configuration)
	$(CXX) $(LDFLAGS) test/test.o libxmlstream.a -o $@

test/test.o: test/test.cpp include/xml_stream.h $(CXXFLAGS_configuration)
	$(CXX) $(CXXFLAGS) -Iinclude -c $< -o $@

.PHONY: default all all-here all-recursive check clean clean-here clean-recursive
