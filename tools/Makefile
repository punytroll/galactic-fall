root = ..

default: all

all: all-recursive

all-here: 

all-recursive: all-here analyze_littlem-all-recursive code_generator-all-recursive rasterizer-all-recursive raw_to_littlem-all-recursive xml_stream-all-recursive

clean: clean-recursive

clean-here: 

clean-recursive: clean-here
	@$(MAKE) -C analyze_littlem clean-recursive
	@$(MAKE) -C code_generator clean-recursive
	@$(MAKE) -C rasterizer clean-recursive
	@$(MAKE) -C raw_to_littlem clean-recursive
	@$(MAKE) -C xml_stream clean-recursive

analyze_littlem: analyze_littlem-all-recursive

code_generator: code_generator-all-recursive

rasterizer: rasterizer-all-recursive

raw_to_littlem: raw_to_littlem-all-recursive

xml_stream: xml_stream-all-recursive

analyze_littlem-all-recursive:
	@$(MAKE) -C analyze_littlem all-recursive

code_generator-all-recursive:
	@$(MAKE) -C code_generator all-recursive

rasterizer-all-recursive:
	@$(MAKE) -C rasterizer all-recursive

raw_to_littlem-all-recursive:
	@$(MAKE) -C raw_to_littlem all-recursive

xml_stream-all-recursive:
	@$(MAKE) -C xml_stream all-recursive

.PHONY: all all-here all-recursive analyze_littlem analyze_littlem-all-recursive clean clean-here clean-recursive code_generator code_generator-all-recursive default rasterizer rasterizer-all-recursive raw_to_littlem raw_to_littlem-all-recursive xml_stream xml_stream-all-recursive
