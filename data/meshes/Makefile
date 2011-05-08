root = ../..

-include $(root)/configuration

include $(root)/tools/raw_to_littlem/Makefile.include

raw_mesh_files = $(wildcard *.raw)
generated_littlem_mesh_files = $(patsubst %.raw,%.xml,$(raw_mesh_files))

default: all

all: all-recursive

all-here: check-dependencies $(generated_littlem_mesh_files)

all-recursive: all-here

check-dependencies: raw_to_littlem-check-dependencies

clean: clean-recursive

clean-here:
	$(RM) fighter_cockpit.xml
	$(RM) fighter_faction.xml
	$(RM) fighter_hull.xml
	$(RM) laser_shot.xml
	$(RM) light_laser.xml
	$(RM) planet.xml
	$(RM) shuttle_faction.xml
	$(RM) shuttle_hull.xml
	$(RM) transporter_faction.xml
	$(RM) transporter_hull.xml

clean-recursive: clean-here

fighter_cockpit.xml: fighter_cockpit.raw Makefile $(raw_to_littlem_dependencies)
	$(command_raw_to_littlem) --in=$< --out=$@ --identifier=fighter_cockpit --smooth

fighter_faction.xml: fighter_faction.raw Makefile $(raw_to_littlem_dependencies)
	$(command_raw_to_littlem) --in=$< --out=$@ --identifier=fighter_faction --smooth

fighter_hull.xml: fighter_hull.raw Makefile $(raw_to_littlem_dependencies)
	$(command_raw_to_littlem) --in=$< --out=$@ --identifier=fighter_hull --smooth

laser_shot.xml: laser_shot.raw Makefile $(raw_to_littlem_dependencies)
	$(command_raw_to_littlem) --in=$< --out=$@ --identifier=laser_shot

light_laser.xml: light_laser.raw Makefile $(raw_to_littlem_dependencies)
	$(command_raw_to_littlem) --in=$< --out=$@ --identifier=light_laser

planet.xml: planet.raw Makefile $(raw_to_littlem_dependencies)
	$(command_raw_to_littlem) --in=$< --out=$@ --identifier=planet --smooth

shuttle_faction.xml: shuttle_faction.raw Makefile $(raw_to_littlem_dependencies)
	$(command_raw_to_littlem) --in=$< --out=$@ --identifier=shuttle_faction --smooth

shuttle_hull.xml: shuttle_hull.raw Makefile $(raw_to_littlem_dependencies)
	$(command_raw_to_littlem) --in=$< --out=$@ --identifier=shuttle_hull --smooth

transporter_faction.xml: transporter_faction.raw Makefile $(raw_to_littlem_dependencies)
	$(command_raw_to_littlem) --in=$< --out=$@ --identifier=transporter_faction

transporter_hull.xml: transporter_hull.raw Makefile $(raw_to_littlem_dependencies)
	$(command_raw_to_littlem) --in=$< --out=$@ --identifier=transporter_hull
