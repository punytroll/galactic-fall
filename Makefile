root = .

default: all

all: build all-recursive

all-here:
	meson compile -C build

all-recursive: all-here data-all-recursive

build:
	meson setup build

check:
	meson test -C build

clean: clean-recursive

clean-here:
	$(RM) -R build

clean-recursive: clean-here
	@$(MAKE) -C data clean-recursive

clean-repository: clean
	$(RM) -R .configuration.d
	$(RM) configuration
	$(RM) -R subprojects/algebra
	$(RM) -R subprojects/expressions
	$(RM) -R subprojects/string_cast
	$(RM) -R subprojects/xml_parser
	$(RM) -R subprojects/xml_puny_dom
	$(RM) -R subprojects/xml_stream

data: data-all-recursive

data-all-recursive:
	@$(MAKE) -C data all-recursive

.PHONY: all all-here all-recursive clean clean-here clean-recursive data data-all-recursive default
