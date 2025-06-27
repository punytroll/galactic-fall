root = ..

default: all

all: all-recursive

all-here: 

all-recursive: all-here rasterizer-all-recursive xml_stream-all-recursive xml_to_data-all-recursive

check: 

clean: clean-recursive

clean-here: 

clean-recursive: clean-here
	@$(MAKE) -C pyarx clean-recursive
	@$(MAKE) -C rasterizer clean-recursive
	@$(MAKE) -C xml_stream clean-recursive
	@$(MAKE) -C xml_to_data clean-recursive

rasterizer: rasterizer-all-recursive

xml_stream: xml_stream-all-recursive

xml_to_data: xml_to_data-all-recursive

rasterizer-all-recursive:
	@$(MAKE) -C rasterizer all-recursive

xml_stream-all-recursive:
	@$(MAKE) -C xml_stream all-recursive

xml_to_data-all-recursive:
	@$(MAKE) -C xml_to_data all-recursive

.PHONY: all all-here all-recursive check clean clean-here clean-recursive default rasterizer rasterizer-all-recursive xml_stream xml_stream-all-recursive xml_to_data xml_to_data-all-recursive
