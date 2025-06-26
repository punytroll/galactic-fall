root = ..

default: all

all: all-recursive

all-here: 

all-recursive: all-here graphics-all-recursive

check:
	@$(MAKE) -C graphics check

clean: clean-recursive

clean-here: 

clean-recursive: clean-here
	@$(MAKE) -C graphics clean-recursive

graphics: graphics-all-recursive

graphics-all-recursive:
	@$(MAKE) -C graphics all-recursive

.PHONY: all all-here all-recursive check clean clean-here clean-recursive arx graphics graphics-all-recursive
