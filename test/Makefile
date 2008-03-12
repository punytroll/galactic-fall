root = ..

-include $(root)/configuration

SOURCE_FILES = $(wildcard *.cpp)
OBJECT_FILES = $(patsubst %.cpp,%.o,$(SOURCE_FILES))

TEST_PROGRAMS = \
	referencing

referencing_test = \
	$(root)/source/referencing.h

all: $(TEST_PROGRAMS)

check: all
	@for TEST_PROGRAM in $(TEST_PROGRAMS); do \
		echo "Running test program" $${TEST_PROGRAM}; \
		./$${TEST_PROGRAM}; \
	done

clean:
	$(RM) $(OBJECT_FILES)
	$(RM) $(TEST_PROGRAMS)

referencing: referencing.o
	$(CXX) $(LDFLAGS) $^ -o $@

referencing.o: referencing.cpp $(referencing_test)
	$(CXX) $(CXXFLAGS) -c $< -o $@
