root = ..

default: all

all: all-recursive

all-here: 

all-recursive: all-here arx-all-recursive events-all-recursive graphics-all-recursive xml_stream-all-recursive

check:
	@$(MAKE) -C arx check
	@$(MAKE) -C events check
	@$(MAKE) -C graphics check
	@$(MAKE) -C xml_stream check

clean: clean-recursive

clean-here: 

clean-recursive: clean-here
	@$(MAKE) -C arx clean-recursive
	@$(MAKE) -C events clean-recursive
	@$(MAKE) -C graphics clean-recursive
	@$(MAKE) -C xml_stream clean-recursive

arx: arx-all-recursive

events: events-all-recursive

graphics: graphics-all-recursive

xml_stream: xml_stream-all-recursive

arx-all-recursive:
	@$(MAKE) -C arx all-recursive

events-all-recursive:
	@$(MAKE) -C events all-recursive

graphics-all-recursive:
	@$(MAKE) -C graphics all-recursive

xml_stream-all-recursive:
	@$(MAKE) -C xml_stream all-recursive

.PHONY: all all-here all-recursive check clean clean-here clean-recursive arx arx-all-recursive events events-all-recursive graphics graphics-all-recursive xml_stream xml_stream-all-recursive
