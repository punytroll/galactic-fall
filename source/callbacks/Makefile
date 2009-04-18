root = ../..

-include $(root)/configuration

include $(root)/tools/code_generator/Makefile.include

callbacks_inc_generated_code_files = \
	.generated-code/Counter.gc \
	.generated-code/CallbackCore0.gc \
	.generated-code/CallbackCore1.gc \
	.generated-code/FunctionCore0.gc \
	.generated-code/FunctionCore1.gc \
	.generated-code/MethodCore0.gc \
	.generated-code/MethodCore1.gc \
	.generated-code/Callback0.gc \
	.generated-code/Callback1.gc \
	.generated-code/BindArgument1Core0.gc \
	.generated-code/FunctionCallback0.gc \
	.generated-code/FunctionCallback1.gc \
	.generated-code/MethodCallback0.gc \
	.generated-code/MethodCallback1.gc \
	.generated-code/Bind1_0.gc

events_inc_generated_code_files = \
	.generated-code/Event0.gc \
	.generated-code/Event1.gc

assertions = false
instance_count = false

default: all-recursive

all: all-recursive

all-here: check-dependencies .generated-code callbacks.inc events.inc

all-recursive: all-here

check-dependencies: code_generator-check-dependencies

clean: clean-recursive

clean-here: 
	$(RM) .generated-code/*
	rmdir .generated-code

clean-recursive: clean-here

callbacks.inc: $(callbacks_inc_generated_code_files)
	@$(RM) $@
	@for GENERATED_CODE_FILE in $^; do \
		cat $${GENERATED_CODE_FILE} >> $@; \
		echo >> $@; \
	done;

events.inc: $(events_inc_generated_code_files)
	@$(RM) $@
	@for GENERATED_CODE_FILE in $^; do \
		cat $${GENERATED_CODE_FILE} >> $@; \
		echo >> $@; \
	done;

.generated-code:
	mkdir $@

.generated-code/Bind1_0.gc: code-templates/Bind.cgt $(code_generator_dependencies) Makefile
	$(command_code_generator) --in=$< --out=$@ --variable=BindArgument=1 --variable=NumberOfArguments=1

.generated-code/BindArgument1Core0.gc: code-templates/BindArgumentCore.cgt $(code_generator_dependencies) Makefile
	$(command_code_generator) --in=$< --out=$@ --variable=Assertions=$(assertions) --variable=BindArgument=1 --variable=InstanceCount=$(instance_count) --variable=NumberOfArguments=1

.generated-code/Callback0.gc: code-templates/Callback.cgt $(code_generator_dependencies) Makefile
	$(command_code_generator) --in=$< --out=$@ --variable=Assertions=$(assertions) --variable=InstanceCount=$(instance_count) --variable=NumberOfArguments=0

.generated-code/Callback1.gc: code-templates/Callback.cgt $(code_generator_dependencies) Makefile
	$(command_code_generator) --in=$< --out=$@ --variable=Assertions=$(assertions) --variable=InstanceCount=$(instance_count) --variable=NumberOfArguments=1

.generated-code/CallbackCore0.gc: code-templates/CallbackCore.cgt $(code_generator_dependencies) Makefile
	$(command_code_generator) --in=$< --out=$@ --variable=InstanceCount=$(instance_count) --variable=NumberOfArguments=0

.generated-code/CallbackCore1.gc: code-templates/CallbackCore.cgt $(code_generator_dependencies) Makefile
	$(command_code_generator) --in=$< --out=$@ --variable=InstanceCount=$(instance_count) --variable=NumberOfArguments=1

.generated-code/Counter.gc: code-templates/Counter.cgt $(code_generator_dependencies) Makefile
	$(command_code_generator) --in=$< --out=$@ --variable=Assertions=$(assertions) --variable=InstanceCount=$(instance_count)

.generated-code/Event0.gc: code-templates/Event.cgt $(code_generator_dependencies) Makefile
	$(command_code_generator) --in=$< --out=$@ --variable=Assersions=$(assertions) --variable=InstanceCount=$(instance_count) --variable=NumberOfArguments=0

.generated-code/Event1.gc: code-templates/Event.cgt $(code_generator_dependencies) Makefile
	$(command_code_generator) --in=$< --out=$@ --variable=Assersions=$(assertions) --variable=InstanceCount=$(instance_count) --variable=NumberOfArguments=1

.generated-code/FunctionCallback0.gc: code-templates/FunctionCallback.cgt $(code_generator_dependencies) Makefile
	$(command_code_generator) --in=$< --out=$@ --variable=NumberOfArguments=0

.generated-code/FunctionCallback1.gc: code-templates/FunctionCallback.cgt $(code_generator_dependencies) Makefile
	$(command_code_generator) --in=$< --out=$@ --variable=NumberOfArguments=1

.generated-code/FunctionCore0.gc: code-templates/FunctionCore.cgt $(code_generator_dependencies) Makefile
	$(command_code_generator) --in=$< --out=$@ --variable=Assertions=$(assertions) --variable=InstanceCount=$(instance_count) --variable=NumberOfArguments=0

.generated-code/FunctionCore1.gc: code-templates/FunctionCore.cgt $(code_generator_dependencies) Makefile
	$(command_code_generator) --in=$< --out=$@ --variable=Assertions=$(assertions) --variable=InstanceCount=$(instance_count) --variable=NumberOfArguments=1

.generated-code/MethodCallback0.gc: code-templates/MethodCallback.cgt $(code_generator_dependencies) Makefile
	$(command_code_generator) --in=$< --out=$@ --variable=NumberOfArguments=0

.generated-code/MethodCallback1.gc: code-templates/MethodCallback.cgt $(code_generator_dependencies) Makefile
	$(command_code_generator) --in=$< --out=$@ --variable=NumberOfArguments=1

.generated-code/MethodCore0.gc: code-templates/MethodCore.cgt $(code_generator_dependencies) Makefile
	$(command_code_generator) --in=$< --out=$@ --variable=Assertions=$(assertions) --variable=InstanceCount=$(instance_count) --variable=NumberOfArguments=0

.generated-code/MethodCore1.gc: code-templates/MethodCore.cgt $(code_generator_dependencies) Makefile
	$(command_code_generator) --in=$< --out=$@ --variable=Assertions=$(assertions) --variable=InstanceCount=$(instance_count) --variable=NumberOfArguments=1

.PHONY: all all-here all-recursive clean clean-here clean-recursive default
