all: all-recursive

all-recursive: data source

clean: clean-recursive

clean-recursive:
	@$(MAKE) -C data clean
	@$(MAKE) -C source clean

source data:
	@$(MAKE) -C $@ all

install: all
	@echo -e "This project is not installable yet. Please run \"./galactic-fall\" from the top directory."

.PHONY: all all-recursive clean clean-recursive data install source
