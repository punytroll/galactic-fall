root = ..

-include $(root)/configuration

default: all

include $(root)/libraries/graphics/Makefile.include
include $(root)/tools/generate_cpp_resource_constants/Makefile.include

HEADER_DIRECTORIES := $(patsubst %/,%,$(sort $(dir $(shell find . -name '*.h'))))
HEADER_FILES := $(patsubst ./%,%,$(foreach HEADER_DIRECTORY,$(HEADER_DIRECTORIES),$(wildcard $(HEADER_DIRECTORY)/*.h)))
SOURCE_DIRECTORIES := $(patsubst %/,%,$(sort $(dir $(shell find . -name '*.cpp'))))
SOURCE_FILES := $(patsubst ./%,%,$(foreach SOURCE_DIRECTORY,$(SOURCE_DIRECTORIES),$(wildcard $(SOURCE_DIRECTORY)/*.cpp)))
OBJECT_DIRECTORY := .object-files
OBJECT_DIRECTORIES := $(patsubst .%, $(OBJECT_DIRECTORY)%, $(SOURCE_DIRECTORIES))
OBJECT_FILES := $(sort $(patsubst %.cpp,$(OBJECT_DIRECTORY)/%.o,$(SOURCE_FILES)))
DEPENDENCY_DIRECTORY := .dependency-files
DEPENDENCY_DIRECTORIES := $(sort $(patsubst .%, $(DEPENDENCY_DIRECTORY)%, $(SOURCE_DIRECTORIES)))
DEPENDENCY_FILES := $(sort $(foreach file,$(SOURCE_FILES),$(DEPENDENCY_DIRECTORY)/$(file).dep))
GENERATED_HEADER_FILES := arx_types.h
GENERATED_FILES := $(GENERATED_HEADER_FILES)

ifeq (0, $(words $(findstring $(MAKECMDGOALS), clean clean-recursive clean-here)))
    -include $(DEPENDENCY_FILES)
endif

all: all-recursive

all-here: check-dependencies binary

all-recursive: all-here

clean: clean-recursive

clean-here:
	$(RM) $(GENERATED_FILES)
	$(RM) binary
	$(RM) $(OBJECT_FILES)
	@MORE_OBJECT_FILES="$(filter-out $(OBJECT_FILES),$(wildcard $(OBJECT_DIRECTORY)/*.o))"; \
	if [ x"$${MORE_OBJECT_FILES}" != x ]; then \
		echo "\nThe following object files have not been removed explicitly. They are deleted with the \"$(OBJECT_DIRECTORY)\" directory, nevertheless."; \
		for OBJECT_FILE in $${MORE_OBJECT_FILES}; do \
			echo "    $${OBJECT_FILE}"; \
		done; \
		echo ""; \
	fi
	$(RM) -r $(OBJECT_DIRECTORY)
	$(RM) $(DEPENDENCY_FILES)
	$(RM) -r $(DEPENDENCY_DIRECTORY)

clean-recursive: clean-here

check-dependencies: generate_cpp_resource_constants-check-dependencies

arx_types.h: $(root)/data/tools/declarations.xml $(generate_cpp_resource_constants_dependencies)
	$(command_generate_cpp_resource_constants) --declarations=$< > $@

binary: $(OBJECT_FILES) $(root)/build/libraries/arxx/libarxx.a $(root)/build/libraries/events/libevents.a $(root)/build/subprojects/expressions/libexpressions.a $(root)/build/subprojects/string_cast/libstring_cast.a $(root)/build/subprojects/xml_parser/libxml_parser.a $(root)/build/subprojects/xml_puny_dom/libxml_puny_dom.a $(root)/build/subprojects/xml_stream/libxml_stream.a $(LDFLAGS_configuration)
	$(CXX) $(LDFLAGS) $(OBJECT_FILES) $(root)/build/libraries/arxx/libarxx.a $(root)/build/libraries/events/libevents.a $(root)/build/subprojects/expressions/libexpressions.a $(GRAPHICS_LIBRARY) $(root)/build/subprojects/string_cast/libstring_cast.a $(root)/build/subprojects/xml_puny_dom/libxml_puny_dom.a $(root)/build/subprojects/xml_parser/libxml_parser.a $(root)/build/subprojects/xml_stream/libxml_stream.a -lGL -lX11 -o $@

$(OBJECT_FILES): $(CXXFLAGS_configuration)

$(DEPENDENCY_DIRECTORY)/%.dep: % $(GENERATED_FILES)
	@test -d $(@D) || mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -MM -MP $< -I$(root)/subprojects/algebra/include -I$(root)/libraries/arxx/include -I$(root)/libraries/events/include -I$(root)/subprojects/expressions/include $(GRAPHICS_INCLUDE) -I$(root)/subprojects/string_cast/include -I$(root)/subprojects/xml_puny_dom/include -I$(root)/subprojects/xml_stream/include | sed 's/^\(.*\.o\):/$(OBJECT_DIRECTORY)\/$(subst /,\/,$(subst ./,,$(dir $<)))\1 $(subst /,\/,$@):/g' > $@

$(OBJECT_DIRECTORY)/%.o: %.cpp
	@test -d $(@D) || mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c $< -I$(root)/subprojects/algebra/include -I$(root)/libraries/arxx/include -I$(root)/libraries/events/include -I$(root)/subprojects/expressions/include $(GRAPHICS_INCLUDE) -I$(root)/subprojects/string_cast/include -I$(root)/subprojects/xml_puny_dom/include -I$(root)/subprojects/xml_stream/include -o $@

.PHONY: all all-here all-recursive clean clean-here clean-recursive check-dependencies default
