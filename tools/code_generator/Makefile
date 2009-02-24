root = ../..

default: all

all: all-recursive

all-here: 

all-recursive: all-here

clean: clean-recursive

clean-here:
	$(RM) CodeModel.pyc
	$(RM) Context.pyc
	$(RM) Functors.pyc
	$(RM) Template.pyc

clean-recursive: clean-here

.PHONY: all all-here all-recursive clean clean-here clean-recursive default
