root = ../..

default: all

-include $(root)/configuration

all: all-recursive

all-here: libarx.a

all-recursive: all-here

check: test/test
	./test/test > /dev/null

clean: clean-recursive

clean-here:
	$(RM) libarx.a
	$(RM) source/Archive.o
	$(RM) source/Buffer.o
	$(RM) source/BufferReader.o
	$(RM) source/BufferWriter.o
	$(RM) source/Common.o
	$(RM) source/Data.o
	$(RM) source/Item.o
	$(RM) source/Reference.o
	$(RM) source/ReferenceImplementation.o
	$(RM) source/Relation.o
	$(RM) source/Structure.o

clean-recursive: clean-here

source/Archive.o: source/Archive.cxx include/Archive.h $(CXXFLAGS_configuration)
	$(CXX) $(CXXFLAGS) -Iinclude -c $< -o $@

source/Buffer.o: source/Buffer.cxx include/Buffer.h $(CXXFLAGS_configuration)
	$(CXX) $(CXXFLAGS) -Iinclude -c $< -o $@

source/BufferReader.o: source/BufferReader.cxx include/BufferReader.h $(CXXFLAGS_configuration)
	$(CXX) $(CXXFLAGS) -Iinclude -c $< -o $@

source/BufferWriter.o: source/BufferWriter.cxx include/BufferWriter.h $(CXXFLAGS_configuration)
	$(CXX) $(CXXFLAGS) -Iinclude -c $< -o $@

source/Common.o: source/Common.cxx include/Common.h $(CXXFLAGS_configuration)
	$(CXX) $(CXXFLAGS) -Iinclude -c $< -o $@

source/Data.o: source/Data.cxx include/Data.h $(CXXFLAGS_configuration)
	$(CXX) $(CXXFLAGS) -Iinclude -c $< -o $@

source/Item.o: source/Item.cxx include/Item.h $(CXXFLAGS_configuration)
	$(CXX) $(CXXFLAGS) -Iinclude -c $< -o $@

source/Reference.o: source/Reference.cxx include/Reference.h $(CXXFLAGS_configuration)
	$(CXX) $(CXXFLAGS) -Iinclude -c $< -o $@

source/ReferenceImplementation.o: source/ReferenceImplementation.cxx source/ReferenceImplementation.h $(CXXFLAGS_configuration)
	$(CXX) $(CXXFLAGS) -Iinclude -c $< -o $@

source/Relation.o: source/Relation.cxx $(CXXFLAGS_configuration)
	$(CXX) $(CXXFLAGS) -Iinclude -c $< -o $@

source/Structure.o: source/Structure.cxx include/Structure.h $(CXXFLAGS_configuration)
	$(CXX) $(CXXFLAGS) -Iinclude -c $< -o $@

libarx.a: source/Archive.o source/Buffer.o source/BufferReader.o source/BufferWriter.o source/Common.o source/Data.o source/Item.o source/Reference.o source/ReferenceImplementation.o source/Relation.o source/Structure.o
	ar rcs $@ $^

.PHONY: default all all-here all-recursive check clean clean-here clean-recursive
