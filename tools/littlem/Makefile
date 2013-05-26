root = ../..

default: all

all: all-recursive

all-here: 

all-recursive: all-here source-all-recursive

clean: clean-recursive

clean-here: 

clean-recursive:
	@$(MAKE) -C source clean-recursive

clean-repository: clean

source: source-all-recursive

source-all-recursive:
	@$(MAKE) -C source all-recursive

.PHONY: default all all-here clean clean-here clean-repository source
