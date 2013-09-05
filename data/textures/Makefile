root = ../..

-include $(root)/configuration

default: all

include $(root)/tools/rasterizer/Makefile.include

vg_files = $(wildcard original/vg/*.vg)
tex_files = $(patsubst original/vg/%.vg,generated/tex/%.tex,$(vg_files))

all: all-recursive

all-here: check-dependencies $(tex_files)

all-recursive: all-here

check-dependencies: rasterizer-check-dependencies

clean: clean-recursive

clean-here:
	$(RM) -rfv generated

clean-recursive: clean-here

generated/tex/particle.tex: original/vg/particle.vg $(rasterizer_dependencies) generated/tex
	$(command_rasterizer) --in=$< --out=$@ --sub-pixels=3 --dimensions=32x32

generated generated/tex:
	mkdir $@

generated/tex: generated

.PHONY: default all all-here all-recursive check-dependencies clean clean-here clean-recursive
