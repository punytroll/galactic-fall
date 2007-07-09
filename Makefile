all: all-recursive

all-recursive: data binary

clean: clean-recursive
	$(RM) galactic-fall

clean-recursive:
	@$(MAKE) -C data clean
	@$(MAKE) -C source clean

binary: source galactic-fall

galactic-fall: source/binary
	cp $< $@

source data:
	@$(MAKE) -C $@ all

install: all
	@echo -e "This project is not installable yet. Please run \"./galactic-fall\" from the top directory."

.PHONY: all all-recursive binary clean clean-recursive data install source
