all: all-recursive

all-recursive: data galactic-fall

clean: clean-recursive
	$(RM) galactic-fall

clean-recursive:
	@$(MAKE) -C data clean
	@$(MAKE) -C source clean

galactic-fall: source
	cp source/binary $@

source:
	@$(MAKE) -C source all

data:
	@$(MAKE) -C data all

install: all
	@echo -e "This project is not installable yet. Please run \"./galactic-fall\" from the top directory."

.PHONY: data source
