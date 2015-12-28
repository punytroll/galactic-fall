root = ..

default: all

all: all-recursive

all-here: 

all-recursive: all-here algebra-all-recursive arx-all-recursive events-all-recursive string_cast-all-recursive xml_parser-all-recursive xml_puny_dom-all-recursive xml_stream-all-recursive

check:
	@$(MAKE) -C algebra check
	@$(MAKE) -C arx check
	@$(MAKE) -C events check
	@$(MAKE) -C string_cast check
	@$(MAKE) -C xml_parser check
	@$(MAKE) -C xml_puny_dom check
	@$(MAKE) -C xml_stream check

clean: clean-recursive

clean-here: 

clean-recursive: clean-here
	@$(MAKE) -C algebra clean-recursive
	@$(MAKE) -C arx clean-recursive
	@$(MAKE) -C events clean-recursive
	@$(MAKE) -C string_cast clean-recursive
	@$(MAKE) -C xml_parser clean-recursive
	@$(MAKE) -C xml_puny_dom clean-recursive
	@$(MAKE) -C xml_stream clean-recursive

algebra: algebra-all-recursive

arx: arx-all-recursive

events: events-all-recursive

string_cast: string_cast-all-recursive

xml_parser: xml_parser-all-recursive

xml_puny_dom: xml_parser xml_puny_dom-all-recursive

xml_stream: xml_stream-all-recursive

algebra-all-recursive:
	@$(MAKE) -C algebra all-recursive

arx-all-recursive:
	@$(MAKE) -C arx all-recursive

events-all-recursive:
	@$(MAKE) -C events all-recursive

string_cast-all-recursive:
	@$(MAKE) -C string_cast all-recursive

xml_parser-all-recursive:
	@$(MAKE) -C xml_parser all-recursive

xml_puny_dom-all-recursive:
	@$(MAKE) -C xml_puny_dom all-recursive

xml_stream-all-recursive:
	@$(MAKE) -C xml_stream all-recursive

.PHONY: all all-here all-recursive check clean clean-here clean-recursive algebra algebra-all-recursive arx arx-all-recursive events events-all-recursive string_cast string_cast-all-recursive xml_parser xml_parser-all-recursive xml_puny_dom xml_puny_dom-all-recursive xml_stream xml_stream-all-recursive
