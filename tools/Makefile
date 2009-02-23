root = ..

default: all

all: all-recursive

all-here: 

all-recursive: all-here code-generator-all-recursive rasterizer-all-recursive

clean: clean-recursive

clean-here: 

clean-recursive: clean-here
	@$(MAKE) -C code-generator clean-recursive
	@$(MAKE) -C rasterizer clean-recursive

code-generator: code-generator-all-recursive

rasterizer: rasterizer-all-recursive

code-generator-all-recursive:
	@$(MAKE) -C code-generator all-recursive

rasterizer-all-recursive:
	@$(MAKE) -C rasterizer all-recursive

.PHONY: all all-here all-recursive clean clean-here clean-recursive code-generator code-generator-all-recursive default rasterizer rasterizer-all-recursive
