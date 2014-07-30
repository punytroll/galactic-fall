root = ../..

default: all

-include $(root)/configuration

all: all-recursive

all-here: libstringcast.a

all-recursive: all-here

check: test/test
	./test/test > /dev/null

clean: clean-recursive

clean-here:
	$(RM) libstringcast.a
	$(RM) source/string_cast.o
	$(RM) test/test
	$(RM) test/test.o

clean-recursive: clean-here

libstringcast.a: source/string_cast.o
	ar rcs $@ $^

source/string_cast.o: source/string_cast.cpp include/string_cast.h $(CXXFLAGS_configuration)
	$(CXX) $(CXXFLAGS) -Iinclude -c $< -o $@

test/test: test/test.o libstringcast.a $(LDFLAGS_configuration)
	$(CXX) $(LDFLAGS) test/test.o libstringcast.a -o $@

test/test.o: test/test.cpp include/string_cast.h $(CXXFLAGS_configuration)
	$(CXX) $(CXXFLAGS) -Iinclude -c $< -o $@

.PHONY: default all all-here all-recursive check clean clean-here clean-recursive
