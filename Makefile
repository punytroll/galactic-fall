root = .

default: all

all: all-recursive

all-here: configuration

all-recursive: all-here libraries-all-recursive tools-all-recursive data-all-recursive source-all-recursive

check:
	@$(MAKE) -C libraries check
	@$(MAKE) -C tools check

clean: clean-recursive

clean-here: 

clean-recursive: clean-here
	@$(MAKE) -C data clean-recursive
	@$(MAKE) -C libraries clean-recursive
	@$(MAKE) -C source clean-recursive
	@$(MAKE) -C tools clean-recursive

clean-repository: clean
	$(RM) -R .configuration.d
	$(RM) configuration

configuration:
	./configure

data: data-all-recursive

libraries: libraries-all-recursive

source: source-all-recursive

tools: tools-all-recursive

data-all-recursive:
	@$(MAKE) -C data all-recursive

libraries-all-recursive:
	@$(MAKE) -C libraries all-recursive

source-all-recursive:
	@$(MAKE) -C source all-recursive

tools-all-recursive:
	@$(MAKE) -C tools all-recursive

install: all
	@echo -e "This project is not installable yet. Please run \"./galactic-fall\" from the top directory."

.PHONY: all all-here all-recursive clean clean-here clean-recursive data data-all-recursive default install libraries libraries-all-recursive source source-all-recursive tools tools-all-recursive
