root = .

default: all

all: all-recursive

all-here: 

all-recursive: all-here tools-all-recursive data-all-recursive source-all-recursive

check:
	@$(MAKE) -C test check

clean: clean-recursive

clean-here: 

clean-recursive: clean-here
	@$(MAKE) -C data clean-recursive
	@$(MAKE) -C source clean-recursive
	@$(MAKE) -C test clean-recursive
	@$(MAKE) -C tools clean-recursive

clean-repository: clean
	$(RM) -R .configuration
	$(RM) configuration

data: data-all-recursive

source: source-all-recursive

test: test-all-recursive

tools: tools-all-recursive

data-all-recursive:
	@$(MAKE) -C data all-recursive

source-all-recursive:
	@$(MAKE) -C source all-recursive

test-all-recursive:
	@$(MAKE) -C test all-recursive

tools-all-recursive:
	@$(MAKE) -C tools all-recursive

install: all
	@echo -e "This project is not installable yet. Please run \"./galactic-fall\" from the top directory."

.PHONY: all all-here all-recursive clean clean-here clean-recursive data data-all-recursive default install source source-all-recursive test test-all-recursive tools tools-all-recursive
