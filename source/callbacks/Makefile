root = ../..

-include $(root)/configuration

code_generator_path = ../tools/code-generator
code_generator = $(code_generator_path)/CodeGenerator.py

GENERATED_CODE_FILES = \
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

CODE_GENERATOR_DEPENDENCIES = \
	$(code_generator_path)/CodeGenerator.py \
	$(code_generator_path)/CodeModel.py \
	$(code_generator_path)/Context.py \
	$(code_generator_path)/Functors.py \
	$(code_generator_path)/Template.py

ASSERTIONS = false
INSTANCE_COUNT = false

default: all-recursive

all: all-recursive

all-here: .generated-code callbacks.inc

all-recursive: all-here

clean: clean-recursive

clean-here: 

clean-recursive: clean-here

callbacks.inc: $(GENERATED_CODE_FILES)
	@ $(RM) callbacks.inc
	@ for GENERATED_CODE_FILE in ${GENERATED_CODE_FILES}; do \
		cat $${GENERATED_CODE_FILE} >> $@; \
		echo >> $@; \
	done;

.generated-code:
	mkdir $@

.generated-code/Bind1_0.gc: code-templates/Bind.cgt $(CODE_GENERATOR_DEPENDENCIES) Makefile
	python $(code_generator) --in=$< --out=$@ --variable=BindArgument=1 --variable=NumberOfArguments=1

.generated-code/BindArgument1Core0.gc: code-templates/BindArgumentCore.cgt $(CODE_GENERATOR_DEPENDENCIES) Makefile
	python $(code_generator) --in=$< --out=$@ --variable=Assertions=$(ASSERTIONS) --variable=BindArgument=1 --variable=InstanceCount=$(INSTANCE_COUNT) --variable=NumberOfArguments=1

.generated-code/Callback0.gc: code-templates/Callback.cgt $(CODE_GENERATOR_DEPENDENCIES) Makefile
	python $(code_generator) --in=$< --out=$@ --variable=Assertions=$(ASSERTIONS) --variable=InstanceCount=$(INSTANCE_COUNT) --variable=NumberOfArguments=0

.generated-code/Callback1.gc: code-templates/Callback.cgt $(CODE_GENERATOR_DEPENDENCIES) Makefile
	python $(code_generator) --in=$< --out=$@ --variable=Assertions=$(ASSERTIONS) --variable=InstanceCount=$(INSTANCE_COUNT) --variable=NumberOfArguments=1

.generated-code/CallbackCore0.gc: code-templates/CallbackCore.cgt $(CODE_GENERATOR_DEPENDENCIES) Makefile
	python $(code_generator) --in=$< --out=$@ --variable=InstanceCount=$(INSTANCE_COUNT) --variable=NumberOfArguments=0

.generated-code/CallbackCore1.gc: code-templates/CallbackCore.cgt $(CODE_GENERATOR_DEPENDENCIES) Makefile
	python $(code_generator) --in=$< --out=$@ --variable=InstanceCount=$(INSTANCE_COUNT) --variable=NumberOfArguments=1

.generated-code/Counter.gc: code-templates/Counter.cgt $(CODE_GENERATOR_DEPENDENCIES) Makefile
	python $(code_generator) --in=$< --out=$@ --variable=Assertions=$(ASSERTIONS) --variable=InstanceCount=$(INSTANCE_COUNT)

.generated-code/FunctionCallback0.gc: code-templates/FunctionCallback.cgt $(CODE_GENERATOR_DEPENDENCIES) Makefile
	python $(code_generator) --in=$< --out=$@ --variable=NumberOfArguments=0

.generated-code/FunctionCallback1.gc: code-templates/FunctionCallback.cgt $(CODE_GENERATOR_DEPENDENCIES) Makefile
	python $(code_generator) --in=$< --out=$@ --variable=NumberOfArguments=1

.generated-code/FunctionCore0.gc: code-templates/FunctionCore.cgt $(CODE_GENERATOR_DEPENDENCIES) Makefile
	python $(code_generator) --in=$< --out=$@ --variable=Assertions=$(ASSERTIONS) --variable=InstanceCount=$(INSTANCE_COUNT) --variable=NumberOfArguments=0

.generated-code/FunctionCore1.gc: code-templates/FunctionCore.cgt $(CODE_GENERATOR_DEPENDENCIES) Makefile
	python $(code_generator) --in=$< --out=$@ --variable=Assertions=$(ASSERTIONS) --variable=InstanceCount=$(INSTANCE_COUNT) --variable=NumberOfArguments=1

.generated-code/MethodCallback0.gc: code-templates/MethodCallback.cgt $(CODE_GENERATOR_DEPENDENCIES) Makefile
	python $(code_generator) --in=$< --out=$@ --variable=NumberOfArguments=0

.generated-code/MethodCallback1.gc: code-templates/MethodCallback.cgt $(CODE_GENERATOR_DEPENDENCIES) Makefile
	python $(code_generator) --in=$< --out=$@ --variable=NumberOfArguments=1

.generated-code/MethodCore0.gc: code-templates/MethodCore.cgt $(CODE_GENERATOR_DEPENDENCIES) Makefile
	python $(code_generator) --in=$< --out=$@ --variable=Assertions=$(ASSERTIONS) --variable=InstanceCount=$(INSTANCE_COUNT) --variable=NumberOfArguments=0

.generated-code/MethodCore1.gc: code-templates/MethodCore.cgt $(CODE_GENERATOR_DEPENDENCIES) Makefile
	python $(code_generator) --in=$< --out=$@ --variable=Assertions=$(ASSERTIONS) --variable=InstanceCount=$(INSTANCE_COUNT) --variable=NumberOfArguments=1

.PHONY: all all-here all-recursive clean clean-here clean-recursive default
