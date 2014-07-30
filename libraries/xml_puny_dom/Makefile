root = ../..

default: all

-include $(root)/configuration
include $(root)/libraries/xml_parser/Makefile.include

all: all-recursive

all-here: libxmlpunydom.a

all-recursive: all-here

check:

clean: clean-recursive

clean-here:
	$(RM) libxmlpunydom.a
	$(RM) source/xml_puny_dom.o

clean-recursive: clean-here

libxmlpunydom.a: source/xml_puny_dom.o $(XML_PARSER_OBJECT_FILES)
	ar rcs $@ $^

source/xml_puny_dom.o: source/xml_puny_dom.cpp include/xml_puny_dom.h $(CXXFLAGS_configuration)
	$(CXX) $(CXXFLAGS) $(XML_PARSER_INCLUDE) -Iinclude -c $< -o $@

.PHONY: default all all-here all-recursive check clean clean-here clean-recursive
