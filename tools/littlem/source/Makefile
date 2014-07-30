root = ../../..

-include $(root)/configuration

default: all

include $(root)/tools/cpp_includes_to_makefile/Makefile.include
include $(root)/libraries/algebra/Makefile.include
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

all: all-recursive

all-here: prepare-build binary

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

dependencies: $(DEPENDENCY_FILES)
	$(RM) Makefile.dependencies
	@for FILE in $(DEPENDENCY_FILES); do \
		echo "cat $${FILE} >> Makefile.dependencies"; \
		cat $${FILE} >> Makefile.dependencies; \
		echo >> Makefile.dependencies; \
	done

$(OBJECT_DIRECTORIES) $(DEPENDENCY_DIRECTORIES):
	mkdir $@

prepare-build: $(OBJECT_DIRECTORIES)

$(OBJECT_FILES): $(CXXFLAGS_configuration)

$(DEPENDENCY_FILES): $(cpp_includes_to_makefile_dependencies) $(DEPENDENCY_DIRECTORIES)

binary: $(OBJECT_FILES) $(XML_STREAM_LIBRARY) $(LDFLAGS_configuration)
	$(CXX) $(LDFLAGS) $(OBJECT_FILES) $(XML_STREAM_LIBRARY) -lGL -lX11 -lglut -lGLU -o $@

.dependency-files/%.dep: %
	$(command_cpp_includes_to_makefile) $< > $@

.SECONDEXPANSION:
$(OBJECT_DIRECTORY)/%.o: $$($$(subst /,_,$$*)_cpp)
	$(CXX) $(CXXFLAGS) $(ALGEBRA_INCLUDE) $(XML_STREAM_INCLUDE)	-c $< -o $@

.PHONY: dependencies
