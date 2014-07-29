root = ../..

default: all

-include $(root)/configuration
include $(root)/libraries/xml_parser/Makefile.include

all: all-recursive

all-here: source/xml_puny_dom.o

all-recursive: all-here

check:

clean: clean-recursive

clean-here: 
	$(RM) source/xml_puny_dom.o

clean-recursive: clean-here

source/xml_puny_dom.o: source/xml_puny_dom.cpp include/xml_puny_dom.h $(CXXFLAGS_configuration)
	$(CXX) $(CXXFLAGS) $(XML_PARSER_INCLUDE) -Iinclude -c $< -o $@

.PHONY: default all all-here all-recursive check clean clean-here clean-recursive
