root = ..

-include $(root)/configuration

default: all

include $(root)/tools/cpp_includes_to_makefile/Makefile.include
include $(root)/libraries/algebra/Makefile.include
include $(root)/libraries/events/Makefile.include
include $(root)/libraries/string_cast/Makefile.include
include $(root)/libraries/xml_parser/Makefile.include
include $(root)/libraries/xml_puny_dom/Makefile.include
include $(root)/libraries/xml_stream/Makefile.include
include Makefile.dependencies

HEADER_DIRECTORIES := $(patsubst %/,%,$(sort $(dir $(shell find . -name '*.h'))))
HEADER_FILES := $(patsubst ./%,%,$(foreach HEADER_DIRECTORY,$(HEADER_DIRECTORIES),$(wildcard $(HEADER_DIRECTORY)/*.h)))
SOURCE_DIRECTORIES := $(patsubst %/,%,$(sort $(dir $(shell find . -name '*.cpp'))))
SOURCE_FILES := $(patsubst ./%,%,$(foreach SOURCE_DIRECTORY,$(SOURCE_DIRECTORIES),$(wildcard $(SOURCE_DIRECTORY)/*.cpp)))
OBJECT_DIRECTORY := .object-files
OBJECT_DIRECTORIES := $(patsubst .%, $(OBJECT_DIRECTORY)%, $(SOURCE_DIRECTORIES))
OBJECT_FILES := $(sort $(patsubst %.cpp,$(OBJECT_DIRECTORY)/%.o,$(SOURCE_FILES)))
DEPENDENCY_DIRECTORY := .dependency-files
DEPENDENCY_DIRECTORIES := $(sort $(patsubst .%, $(DEPENDENCY_DIRECTORY)%, $(HEADER_DIRECTORIES) $(SOURCE_DIRECTORIES)))
DEPENDENCY_FILES := $(sort $(foreach file,$(HEADER_FILES) $(SOURCE_FILES),$(DEPENDENCY_DIRECTORY)/$(file).dep))

ARXX_INCLUDE = $(shell pkg-config --cflags libarxx)
ARXX_LIBRARIES = $(shell pkg-config --libs libarxx)

all: all-recursive

all-here: check-dependencies binary

all-recursive: all-here

clean: clean-recursive

clean-here:
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

check-dependencies: cpp_includes_to_makefile-check-dependencies
	@if [ x`which pkg-config` = x ]; then \
		echo -e "\nYour system is missing the \"pkg-config\" tool.\nThis tool is needed and you will need to install it in order to build the program.\n"; false; \
	fi
	@if ! pkg-config --exists libarxx; then \
		echo -e "\nYour system is missing the \"libarxx\" library.\nYou can get this library from \"http://libarxx.sourceforge.net\"./\n"; false; \
	fi
	@if ! pkg-config --atleast-version=0.8.9 libarxx; then \
		echo -e "\nThe library \"libarxx\" that is installed on your system is too old.\nYou have: Version `pkg-config --modversion libarxx`\nYou need: Version 0.8.9\nYou can get the most recent library from \"http://libarxx.sourceforge.net\"./\n"; false; \
	fi

dependencies: $(DEPENDENCY_FILES)
	$(RM) Makefile.dependencies
	@for FILE in $(DEPENDENCY_FILES); do \
		echo "cat $${FILE} >> Makefile.dependencies"; \
		cat $${FILE} >> Makefile.dependencies; \
		echo >> Makefile.dependencies; \
	done

%.o: %.cpp

$(OBJECT_FILES): $(CXXFLAGS_configuration)

$(DEPENDENCY_FILES): $(cpp_includes_to_makefile_dependencies)

binary: $(OBJECT_FILES) $(EVENTS_LIBRARY) $(STRING_CAST_LIBRARY) $(XML_PUNY_DOM_LIBRARY) $(XML_STREAM_LIBRARY) $(LDFLAGS_configuration)
	$(CXX) $(LDFLAGS) $(OBJECT_FILES) $(ARXX_LIBRARIES) $(EVENTS_LIBRARY) $(STRING_CAST_LIBRARY) $(XML_PUNY_DOM_LIBRARY) $(XML_STREAM_LIBRARY) -lGL -lX11 -o $@

$(DEPENDENCY_DIRECTORY)/%.dep: %
	@test -d $(@D) || mkdir $(@D)
	$(command_cpp_includes_to_makefile) $< > $@

.SECONDEXPANSION:
$(OBJECT_DIRECTORY)/%.o: $$($$(subst /,_,$$*)_cpp)
	@test -d $(@D) || mkdir $(@D)
	$(CXX) $(CXXFLAGS) $(ALGEBRA_INCLUDE) $(ARXX_INCLUDE) $(EVENTS_INCLUDE) $(STRING_CAST_INCLUDE) $(XML_PUNY_DOM_INCLUDE) $(XML_STREAM_INCLUDE) -c $< -o $@

.PHONY: dependencies
