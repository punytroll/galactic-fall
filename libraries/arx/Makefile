root = ../..

-include $(root)/configuration

default: all

HEADER_DIRECTORIES := $(patsubst %/,%,$(sort $(dir $(shell find . -name '*.h'))))
HEADER_FILES := $(patsubst ./%,%,$(foreach HEADER_DIRECTORY,$(HEADER_DIRECTORIES),$(wildcard $(HEADER_DIRECTORY)/*.h)))
SOURCE_DIRECTORIES := $(patsubst %/,%,$(sort $(dir $(shell find . -name '*.cxx'))))
SOURCE_FILES := $(patsubst ./%,%,$(foreach SOURCE_DIRECTORY,$(SOURCE_DIRECTORIES),$(wildcard $(SOURCE_DIRECTORY)/*.cxx)))
OBJECT_DIRECTORY := source/.object-files
OBJECT_DIRECTORIES := $(patsubst .%, $(OBJECT_DIRECTORY)%, $(SOURCE_DIRECTORIES))
OBJECT_FILES := $(sort $(patsubst %.cxx,$(OBJECT_DIRECTORY)/%.o,$(SOURCE_FILES)))
DEPENDENCY_DIRECTORY := source/.dependency-files
DEPENDENCY_DIRECTORIES := $(sort $(patsubst .%, $(DEPENDENCY_DIRECTORY)%, $(SOURCE_DIRECTORIES)))
DEPENDENCY_FILES := $(sort $(foreach file,$(SOURCE_FILES),$(DEPENDENCY_DIRECTORY)/$(file).dep))

ifeq (0, $(words $(findstring $(MAKECMDGOALS), clean clean-recursive clean-here)))
    -include $(DEPENDENCY_FILES)
endif

all: all-recursive

all-here: libarx.a

all-recursive: all-here

check:

clean: clean-recursive

clean-here:
	$(RM) libarx.a
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

$(OBJECT_FILES): $(CXXFLAGS_configuration)

$(DEPENDENCY_DIRECTORY)/%.dep: %
	@test -d $(@D) || mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -MM -MP $< -Iinclude | sed 's/^\(.*\.o\):/$(subst /,\/,$(OBJECT_DIRECTORY))\/$(subst /,\/,$(subst ./,,$(dir $<)))\1 $(subst /,\/,$@):/g' > $@

$(OBJECT_DIRECTORY)/%.o: %.cxx
	@test -d $(@D) || mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -Iinclude -c $< -o $@

libarx.a: $(OBJECT_FILES)
	ar rcs $@ $(OBJECT_FILES)

.PHONY: default all all-here all-recursive check clean clean-here clean-recursive
