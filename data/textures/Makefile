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

generated/asset_class_equipment.tex: original/asset_class_equipment.vg $(rasterizer_dependencies) generated
	$(command_rasterizer) --in=$< --out=$@ --sub-pixels=2 --dimensions=100x100

generated/asset_class_food.tex: original/asset_class_food.vg $(rasterizer_dependencies) generated
	$(command_rasterizer) --in=$< --out=$@ --sub-pixels=2 --dimensions=100x100

generated/asset_class_fuel.tex: original/asset_class_fuel.vg $(rasterizer_dependencies) generated
	$(command_rasterizer) --in=$< --out=$@ --sub-pixels=2 --dimensions=100x100

generated/asset_class_industrial.tex: original/asset_class_industrial.vg $(rasterizer_dependencies) generated
	$(command_rasterizer) --in=$< --out=$@ --sub-pixels=2 --dimensions=100x100

generated/asset_class_light_battery.tex: original/asset_class_light_battery.vg $(rasterizer_dependencies) generated
	$(command_rasterizer) --in=$< --out=$@ --sub-pixels=2 --dimensions=100x100

generated/asset_class_light_laser.tex: original/asset_class_light_laser.vg $(rasterizer_dependencies) generated
	$(command_rasterizer) --in=$< --out=$@ --sub-pixels=2 --dimensions=100x100

generated/asset_class_luxury_goods.tex: original/asset_class_luxury_goods.vg $(rasterizer_dependencies) generated
	$(command_rasterizer) --in=$< --out=$@ --sub-pixels=2 --dimensions=100x100

generated/asset_class_medical_supplies.tex: original/asset_class_medical_supplies.vg $(rasterizer_dependencies) generated
	$(command_rasterizer) --in=$< --out=$@ --sub-pixels=2 --dimensions=100x100

generated/asset_class_medium_laser.tex: original/asset_class_medium_laser.vg $(rasterizer_dependencies) generated
	$(command_rasterizer) --in=$< --out=$@ --sub-pixels=2 --dimensions=100x100

generated/asset_class_metal.tex: original/asset_class_metal.vg $(rasterizer_dependencies) generated
	$(command_rasterizer) --in=$< --out=$@ --sub-pixels=2 --dimensions=100x100

generated/asset_class_small_generator.tex: original/asset_class_small_generator.vg $(rasterizer_dependencies) generated
	$(command_rasterizer) --in=$< --out=$@ --sub-pixels=2 --dimensions=100x100

generated:
	mkdir $@

.PHONY: default all all-here all-recursive check-dependencies clean clean-here clean-recursive
