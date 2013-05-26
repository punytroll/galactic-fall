root = ..

default: all

all: all-recursive

all-here: 

all-recursive: all-here algebra-all-recursive

check:
	@$(MAKE) -C algebra check

clean: clean-recursive

clean-here: 

clean-recursive: clean-here
	@$(MAKE) -C algebra clean-recursive

algebra: algebra-all-recursive

algebra-all-recursive:
	@$(MAKE) -C algebra all-recursive

·PHONY: all all-here all-recursive check clean clean-here clean-recursive algebra algebra-all-recursive
