root = ../..

-include $(root)/configuration

default: all

include $(root)/tools/rasterizer/Makefile.include

vg_files = $(wildcard original/*.vg)
tex_files = $(patsubst original/%.vg,generated/%.tex,$(vg_files))

all: all-recursive

all-here: check-dependencies $(tex_files)

all-recursive: all-here

check-dependencies: rasterizer-check-dependencies

clean: clean-recursive

clean-here:
	$(RM) -rfv generated

clean-recursive: clean-here

generated/particle.tex: original/particle.vg $(rasterizer_dependencies) generated
	$(command_rasterizer) --in=$< --out=$@ --sub-pixels=3 --dimensions=32x32

generated:
	mkdir $@

.PHONY: default all all-here all-recursive check-dependencies clean clean-here clean-recursive
