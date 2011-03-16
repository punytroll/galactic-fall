root = ..

default: all

all: all-recursive

all-here: 

all-recursive: all-here analyze_littlem-all-recursive code_generator-all-recursive list_glx_fb_configurations-all-recursive rasterizer-all-recursive raw_to_littlem-all-recursive view_texture-all-recursive xml_stream-all-recursive xml_to_data-all-recursive

clean: clean-recursive

clean-here: 

clean-recursive: clean-here
	@$(MAKE) -C analyze_littlem clean-recursive
	@$(MAKE) -C code_generator clean-recursive
	@$(MAKE) -C list_glx_fb_configurations clean-recursive
	@$(MAKE) -C rasterizer clean-recursive
	@$(MAKE) -C raw_to_littlem clean-recursive
	@$(MAKE) -C view_texture clean-recursive
	@$(MAKE) -C xml_stream clean-recursive
	@$(MAKE) -C xml_to_data clean-recursive

analyze_littlem: analyze_littlem-all-recursive

code_generator: code_generator-all-recursive

list_glx_fb_configurations: list_glx_fb_configurations-all-recursive

rasterizer: rasterizer-all-recursive

raw_to_littlem: raw_to_littlem-all-recursive

view_texture: view_texture-all-recursive

xml_stream: xml_stream-all-recursive

xml_to_data: xml_to_data-all-recursive

analyze_littlem-all-recursive:
	@$(MAKE) -C analyze_littlem all-recursive

code_generator-all-recursive:
	@$(MAKE) -C code_generator all-recursive

list_glx_fb_configurations-all-recursive:
	@$(MAKE) -C list_glx_fb_configurations all-recursive

rasterizer-all-recursive:
	@$(MAKE) -C rasterizer all-recursive

raw_to_littlem-all-recursive:
	@$(MAKE) -C raw_to_littlem all-recursive

view_texture-all-recursive:
	@$(MAKE) -C view_texture all-recursive

xml_stream-all-recursive:
	@$(MAKE) -C xml_stream all-recursive

xml_to_data-all-recursive:
	@$(MAKE) -C xml_to_data all-recursive

.PHONY: all all-here all-recursive analyze_littlem analyze_littlem-all-recursive clean clean-here clean-recursive code_generator code_generator-all-recursive default list_glx_fb_configurations list_glx_fb_configurations-all-recursive rasterizer rasterizer-all-recursive raw_to_littlem raw_to_littlem-all-recursive view_texture view_texture-all-recursive xml_stream xml_stream-all-recursive xml_to_data xml_to_data-all-recursive
