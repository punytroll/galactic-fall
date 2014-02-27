root = ../..

-include $(root)/configuration

include $(root)/tools/code_generator/Makefile.include

events_inc_generated_code_files = \
	.generated-code/Event0.gc \
	.generated-code/Event1.gc \
	.generated-code/Event2.gc \
	.generated-code/Event4.gc

assertions = false
instance_count = false

default: all-recursive

all: all-recursive

all-here: check-dependencies .generated-code events.inc

all-recursive: all-here

check-dependencies: code_generator-check-dependencies

clean: clean-recursive

clean-here: 
	$(RM) -r .generated-code

clean-recursive: clean-here

events.inc: $(events_inc_generated_code_files)
	@$(RM) $@
	@for GENERATED_CODE_FILE in $^; do \
		cat $${GENERATED_CODE_FILE} >> $@; \
		echo >> $@; \
	done;

.generated-code:
	mkdir $@

.generated-code/Event0.gc: code-templates/Event.cgt $(code_generator_dependencies) Makefile
	$(command_code_generator) --in=$< --out=$@ --variable=Assersions=$(assertions) --variable=InstanceCount=$(instance_count) --variable=NumberOfArguments=0

.generated-code/Event1.gc: code-templates/Event.cgt $(code_generator_dependencies) Makefile
	$(command_code_generator) --in=$< --out=$@ --variable=Assersions=$(assertions) --variable=InstanceCount=$(instance_count) --variable=NumberOfArguments=1

.generated-code/Event2.gc: code-templates/Event.cgt $(code_generator_dependencies) Makefile
	$(command_code_generator) --in=$< --out=$@ --variable=Assersions=$(assertions) --variable=InstanceCount=$(instance_count) --variable=NumberOfArguments=2

.generated-code/Event4.gc: code-templates/Event.cgt $(code_generator_dependencies) Makefile
	$(command_code_generator) --in=$< --out=$@ --variable=Assersions=$(assertions) --variable=InstanceCount=$(instance_count) --variable=NumberOfArguments=4

.PHONY: all all-here all-recursive clean clean-here clean-recursive default
