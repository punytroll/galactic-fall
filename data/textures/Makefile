root = ../..

-include $(root)/configuration
-include $(root)/tools/rasterizer/Makefile.include

vector_graphics_files = $(wildcard *.vg)
tex_files = $(patsubst %.vg,%.tex,$(vector_graphics_files))

default: all

all: all-recursive

all-here: check-dependencies $(tex_files)

all-recursive: all-here

check-dependencies:
	@if [ x`which python` = x ]; then \
	    echo -e "\nYour system is missing the python interpreter.\nIn order to build these textures you will need to install it on your system.\n"; false; \
	fi

clean: clean-recursive

clean-here:
	$(RM) $(tex_files)
	@MORE_TEX_FILES="$(filter-out $(tex_files),$(wildcard *.o))"; \
	if [ x"$${MORE_TEX_FILES}" != x ]; then \
		echo -e "\nThe following tex files have not been removed automatically. You may want to remove them by hand."; \
		for TEX_FILE in $${MORE_TEX_FILES}; do \
			echo -e "\t$${TEX_FILE}"; \
		done; \
		echo ""; \
	fi

clean-recursive: clean-here

particle.tex: particle.vg $(rasterizer_dependencies)
	python $(rasterizer_py) --in=$< --out=$@ --sub-pixels=3 --dimensions=32x32
