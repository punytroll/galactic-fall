root = .

all: all-recursive

all-recursive: data source

check:
	@$(MAKE) -C test check

clean: clean-recursive

clean-recursive:
	$(RM) -R .configuration
	$(RM) configuration
	@$(MAKE) -C data clean
	@$(MAKE) -C source clean
	@$(MAKE) -C test clean

source data test:
	@$(MAKE) -C $@ all

install: all
	@echo -e "This project is not installable yet. Please run \"./galactic-fall\" from the top directory."

.PHONY: all all-recursive clean clean-recursive data install source test
