root = .

default: all

all: all-recursive

all-here: 

all-recursive: all-here data-all-recursive source-all-recursive

check:
	@$(MAKE) -C test check

clean: clean-recursive

clean-here:
	$(RM) -R .configuration
	$(RM) configuration

clean-recursive: clean-here
	@$(MAKE) -C data clean-recursive
	@$(MAKE) -C source clean-recursive
	@$(MAKE) -C test clean-recursive

data: data-all-recursive

source: source-all-recursive

test: test-all-recursive

data-all-recursive:
	@$(MAKE) -C data all-recursive

source-all-recursive:
	@$(MAKE) -C source all-recursive

test-all-recursive:
	@$(MAKE) -C test all-recursive

install: all
	@echo -e "This project is not installable yet. Please run \"./galactic-fall\" from the top directory."

.PHONY: all all-recursive clean clean-recursive data data-all-recursive default install source source-all-recursive test test-all-recursive
