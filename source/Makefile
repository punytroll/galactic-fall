root = ..

-include $(root)/configuration

default: all

include $(root)/libraries/algebra/Makefile.include
include $(root)/libraries/arx/Makefile.include
include $(root)/libraries/events/Makefile.include
include $(root)/libraries/expressions/Makefile.include
include $(root)/libraries/graphics/Makefile.include
include $(root)/libraries/xml_parser/Makefile.include
include $(root)/libraries/xml_puny_dom/Makefile.include
include $(root)/libraries/xml_stream/Makefile.include
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

binary: $(OBJECT_FILES) $(EVENTS_LIBRARY) $(EXPRESSIONS_LIBRARY) $(root)/build/subprojects/string_cast/libstring_cast.a $(XML_PUNY_DOM_LIBRARY) $(XML_STREAM_LIBRARY) $(LDFLAGS_configuration)
	$(CXX) $(LDFLAGS) $(OBJECT_FILES) $(ARX_LIBRARY) $(EVENTS_LIBRARY) $(EXPRESSIONS_LIBRARY) $(GRAPHICS_LIBRARY) $(root)/build/subprojects/string_cast/libstring_cast.a $(XML_PUNY_DOM_LIBRARY) $(XML_STREAM_LIBRARY) -lGL -lX11 -o $@

$(OBJECT_FILES): $(CXXFLAGS_configuration)

$(DEPENDENCY_DIRECTORY)/%.dep: % $(GENERATED_FILES)
	@test -d $(@D) || mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -MM -MP $< $(ALGEBRA_INCLUDE) $(ARX_INCLUDE) $(EVENTS_INCLUDE) $(EXPRESSIONS_INCLUDE) $(GRAPHICS_INCLUDE) -I$(root)/subprojects/string_cast/include $(XML_PUNY_DOM_INCLUDE) $(XML_STREAM_INCLUDE) | sed 's/^\(.*\.o\):/$(OBJECT_DIRECTORY)\/$(subst /,\/,$(subst ./,,$(dir $<)))\1 $(subst /,\/,$@):/g' > $@

$(OBJECT_DIRECTORY)/%.o: %.cpp
	@test -d $(@D) || mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c $< $(ALGEBRA_INCLUDE) $(ARX_INCLUDE) $(EVENTS_INCLUDE) $(EXPRESSIONS_INCLUDE) $(GRAPHICS_INCLUDE) -I$(root)/subprojects/string_cast/include $(XML_PUNY_DOM_INCLUDE) $(XML_STREAM_INCLUDE) -o $@

.PHONY: all all-here all-recursive clean clean-here clean-recursive check-dependencies default
