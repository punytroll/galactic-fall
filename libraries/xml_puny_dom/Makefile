root = ../..

default: all

-include $(root)/configuration

all: all-recursive

all-here: libxmlpunydom.a

all-recursive: all-here

check:

clean: clean-recursive

clean-here:
	$(RM) libxmlpunydom.a
	$(RM) source/xml_puny_dom.o

clean-recursive: clean-here

libxmlpunydom.a: source/xml_puny_dom.o $(root)/build/subprojects/xml_parser/libxml_parser.a
	ar rcs $@ $^

source/xml_puny_dom.o: source/xml_puny_dom.cpp include/xml_puny_dom.h $(CXXFLAGS_configuration)
	$(CXX) $(CXXFLAGS) -I$(root)/subprojects/xml_parser/include -Iinclude -c $< -o $@

.PHONY: default all all-here all-recursive check clean clean-here clean-recursive
