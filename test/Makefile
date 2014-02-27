root = ..

-include $(root)/configuration

SOURCE_FILES = $(wildcard *.cpp)
OBJECT_FILES = $(patsubst %.cpp,%.o,$(SOURCE_FILES))

TEST_PROGRAMS = \
	events \
	referencing

events_test = \
	$(root)/source/connection.h \
	$(root)/source/events.h

referencing_test = \
	$(root)/source/referencing.h

default: all

all: all-recursive

all-here: $(TEST_PROGRAMS)

all-recursive: all-here

check: all
	@for TEST_PROGRAM in $(TEST_PROGRAMS); do \
		echo "Running test program" $${TEST_PROGRAM}; \
		./$${TEST_PROGRAM} > /dev/null; \
	done

clean:
	$(RM) $(OBJECT_FILES)
	$(RM) $(TEST_PROGRAMS)

clean-recursive: clean

events: events.o ../source/connection.o
	$(CXX) $(LDFLAGS) $^ -o $@

events.o: events.cpp $(callbacks_test)
	$(CXX) $(CXXFLAGS) -c $< -o $@

referencing: referencing.o
	$(CXX) $(LDFLAGS) $^ -o $@

referencing.o: referencing.cpp $(referencing_test)
	$(CXX) $(CXXFLAGS) -c $< -o $@
