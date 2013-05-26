root = ..

default: all

all: all-recursive

all-here: 

all-recursive: all-here algebra-all-recursive xml_stream-all-recursive

check:
	@$(MAKE) -C algebra check
	@$(MAKE) -C xml_stream check

clean: clean-recursive

clean-here: 

clean-recursive: clean-here
	@$(MAKE) -C algebra clean-recursive
	@$(MAKE) -C xml_stream clean-recursive

algebra: algebra-all-recursive

xml_stream: xml_stream-all-recursive

algebra-all-recursive:
	@$(MAKE) -C algebra all-recursive

xml_stream-all-recursive:
	@$(MAKE) -C xml_stream all-recursive

·PHONY: all all-here all-recursive check clean clean-here clean-recursive algebra algebra-all-recursive xml_stream xml_stream-all-recursive 
