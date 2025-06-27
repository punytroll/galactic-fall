root = .

default: all

all: build all-recursive

all-here: configuration
	meson compile -C build

all-recursive: all-here tools-all-recursive data-all-recursive source-all-recursive

build:
	meson setup build

check:
	meson test -C build
	@$(MAKE) -C tools check

clean: clean-recursive

clean-here:
	@$(RM) -R build

clean-recursive: clean-here
	@$(MAKE) -C data clean-recursive
	@$(MAKE) -C source clean-recursive
	@$(MAKE) -C tools clean-recursive

clean-repository: clean
	$(RM) -R .configuration.d
	$(RM) configuration
	$(RM) -R subprojects/algebra
	$(RM) -R subprojects/expressions
	$(RM) -R subprojects/string_cast
	$(RM) -R subprojects/xml_parser
	$(RM) -R subprojects/xml_puny_dom
	$(RM) -R subprojects/xml_stream

configuration:
	./configure

data: data-all-recursive

source: source-all-recursive

tools: tools-all-recursive

data-all-recursive:
	@$(MAKE) -C data all-recursive

source-all-recursive:
	@$(MAKE) -C source all-recursive

tools-all-recursive:
	@$(MAKE) -C tools all-recursive

install: all
	@echo -e "This project is not installable yet. Please run \"./galactic-fall\" from the top directory."

.PHONY: all all-here all-recursive clean clean-here clean-recursive data data-all-recursive default install source source-all-recursive tools tools-all-recursive
