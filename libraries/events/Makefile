root = ../..

default: all

-include $(root)/configuration

all: all-recursive

all-here: libevents.a

all-recursive: all-here

check: test/test
	./test/test > /dev/null

clean: clean-recursive

clean-here:
	$(RM) libevents.a
	$(RM) source/connection.o
	$(RM) test/test
	$(RM) test/test.o

clean-recursive: clean-here

libevents.a: source/connection.o
	ar rcs $@ $^

source/connection.o: source/connection.cpp include/events/connection.h $(CXXFLAGS_configuration)
	$(CXX) $(CXXFLAGS) -Iinclude/events -c $< -o $@

test/test: libevents.a test/test.o include/events/connection.h include/events/event.h $(LDFLAGS_configuration)
	$(CXX) $(LDFLAGS) test/test.o libevents.a -o $@

test/test.o: test/test.cpp include/events/connection.h include/events/event.h $(CXXFLAGS_configuration)
	$(CXX) $(CXXFLAGS) -Iinclude/events -c $< -o $@

.PHONY: default all all-here all-recursive check clean clean-here clean-recursive
