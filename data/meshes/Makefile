root = ../..

-include $(root)/configuration

default: all

include $(root)/tools/raw_to_littlem/Makefile.include

raw_files = $(wildcard generated/raw/*.raw)
generated_littlem_mesh_files = $(patsubst generated/raw/%.raw,generated/littlem/%.xml,$(raw_files))

all: all-recursive

all-here: prepare-build $(generated_littlem_mesh_files)

all-recursive: all-here

check-dependencies: raw_to_littlem-check-dependencies

clean: clean-recursive

clean-here:
	$(RM) -rfv generated/littlem

clean-recursive: clean-here

generated/littlem/fighter_cockpit.xml: generated/raw/fighter_cockpit.raw Makefile $(raw_to_littlem_dependencies)
	$(command_raw_to_littlem) --in=$< --out=$@ --identifier=fighter_cockpit --smooth

generated/littlem/fighter_faction.xml: generated/raw/fighter_faction.raw Makefile $(raw_to_littlem_dependencies)
	$(command_raw_to_littlem) --in=$< --out=$@ --identifier=fighter_faction --smooth

generated/littlem/fighter_hull.xml: generated/raw/fighter_hull.raw Makefile $(raw_to_littlem_dependencies)
	$(command_raw_to_littlem) --in=$< --out=$@ --identifier=fighter_hull --smooth

generated/littlem/planet.xml: generated/raw/planet.raw Makefile $(raw_to_littlem_dependencies)
	$(command_raw_to_littlem) --in=$< --out=$@ --identifier=planet --smooth

generated/littlem/shuttle_faction.xml: generated/raw/shuttle_faction.raw Makefile $(raw_to_littlem_dependencies)
	$(command_raw_to_littlem) --in=$< --out=$@ --identifier=shuttle_faction --smooth

generated/littlem/shuttle_hull.xml: generated/raw/shuttle_hull.raw Makefile $(raw_to_littlem_dependencies)
	$(command_raw_to_littlem) --in=$< --out=$@ --identifier=shuttle_hull --smooth

generated/littlem/transporter_faction.xml: generated/raw/transporter_faction.raw Makefile $(raw_to_littlem_dependencies)
	$(command_raw_to_littlem) --in=$< --out=$@ --identifier=transporter_faction

generated/littlem/transporter_hull.xml: generated/raw/transporter_hull.raw Makefile $(raw_to_littlem_dependencies)
	$(command_raw_to_littlem) --in=$< --out=$@ --identifier=transporter_hull

generated/littlem:
	mkdir $@

prepare-build: generated/littlem

.PHONY: default all all-here all-recursive check-dependencies clean clean-here clean-recursive prepare-build
