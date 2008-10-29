root = ..

-include $(root)/configuration

data_arx = \
	$(data_location_configuration) \
	Makefile \
	$(subst internal,$(all_data),$(subst external,,$(data_location)))

all_data = \
	.data \
	.data/asset_class_equipment.data \
	.data/asset_class_food.data \
	.data/asset_class_fuel.data \
	.data/asset_class_industrial.data \
	.data/asset_class_light_battery.data \
	.data/asset_class_light_laser.data \
	.data/asset_class_luxury_goods.data \
	.data/asset_class_medical_supplies.data \
	.data/asset_class_medium_laser.data \
	.data/asset_class_metal.data \
	.data/commodity_class_equipment.data \
	.data/commodity_class_food.data \
	.data/commodity_class_fuel.data \
	.data/commodity_class_industrial.data \
	.data/commodity_class_luxury_goods.data \
	.data/commodity_class_medical_supplies.data \
	.data/commodity_class_metal.data \
	.data/label_credits.data \
	.data/label_current_system.data \
	.data/label_energy.data \
	.data/label_fuel.data \
	.data/label_hull.data \
	.data/label_message.data \
	.data/label_system.data \
	.data/label_target.data \
	.data/label_time_warp.data \
	.data/mesh_cargo_cube.data \
	.data/mesh_fighter_cockpit.data \
	.data/mesh_fighter_hull.data \
	.data/mesh_laser_shot.data \
	.data/mesh_light_laser.data \
	.data/mesh_planet.data \
	.data/mesh_shuttle_hull.data \
	.data/mesh_transporter_hull.data \
	.data/model_cargo_cube.data \
	.data/model_fighter.data \
	.data/model_laser_shot.data \
	.data/model_light_laser.data \
	.data/model_planet.data \
	.data/model_shuttle.data \
	.data/model_transporter.data \
	.data/ship_class_shuttle.data \
	.data/ship_class_transporter.data \
	.data/ship_class_fighter.data \
	.data/slot_class_light_battery.data \
	.data/slot_class_light_weapon.data \
	.data/slot_class_medium_weapon.data \
	.data/system_alpha_centauri.data \
	.data/system_rigel.data \
	.data/system_sol.data \
	.data/system_tichel.data \
	.data/system_link_alpha_centauri_sol.data \
	.data/system_link_alpha_centauri_tichel.data \
	.data/system_link_rigel_sol.data \
	.data/system_link_rigel_tichel.data \
	.data/texture_particle.data \
	.data/weapon_class_light_laser.data \
	.data/weapon_class_medium_laser.data \
	.data/widget_mini_map.data \
	.data/widget_mini_map_display.data \
	.data/widget_scanner.data \
	.data/widget_scanner_display.data

smooth_meshes = \
	fighter_cockpit \
	fighter_hull \
	planet \
	shuttle_hull

declarations_xml = \
	tools/declarations.xml

littlem_to_xml_xsl = \
	tools/littlem_to_xml.xsl

rasterize_py = \
	tools/rasterize.py

raw_to_littlem_py = \
	tools/raw_to_littlem.py \
	tools/xml_stream.py

xml_to_data_py = \
	tools/xml_to_data.py

default: all

all: all-recursive

all-here: check-dependencies $(all_data) data.arx

all-recursive: all-here

check-dependencies:
	@if [ x`which pkg-config` = x ]; then \
		echo -e "\nYour system is missing the \"pkg-config\" tool.\nThis tool is needed and you will need to install it in order to build the program.\n"; false; \
	fi
	@if ! pkg-config --atleast-version=0.5.0 arxtools; then \
		echo -e "\nThe tool suite \"arxtools\" that is installed on your system is too old.\nYou have: Version `pkg-config --modversion arxtools`\nYou need: Version 0.5.0\nYou can get the most recent library from \"http://libarxx.sourceforge.net\"./\n"; false; \
	fi
	@if [ x`which arxtouch` = x -o x`which arxmod` = x -o x`which arxadd` = x ]; then \
	    echo -e "\nYour system is missing parts of the arxtools suite.\nYou will need to install the tools \"arxtouch\", \"arxmod\" and \"arxadd\" on your system.\n"; false; \
	fi
	@if [ x`which python` = x ]; then \
	    echo -e "\nYour system is missing the python interpreter.\nIn order to build the data file you will need to install it on your system.\n"; false; \
	fi
	@if [ x`which xsltproc` = x ]; then \
	    echo -e "\nYour system is missing the xsltproc program.\nTo build the data file you will need to install it on your system.\n"; false; \
	fi
	@if [ x$(data_location) = x ]; then \
		echo -e "\nThe \"data_location\" configuration is missing.\nPlease run the configure script in the top level directory.\n"; false; \
	fi

clean: clean-recursive

clean-here:
	$(RM) -r .data
	$(RM) data.arx
	$(RM) tools/xml_stream.pyc
	$(RM) mesh_cargo_cube.xml
	$(RM) mesh_fighter.xml
	$(RM) mesh_laser_shot.xml
	$(RM) mesh_light_laser.xml
	$(RM) mesh_planet.xml
	$(RM) mesh_shuttle.xml
	$(RM) mesh_transporter.xml
	$(RM) meshes/fighter.xml
	$(RM) meshes/light_laser.xml
	$(RM) meshes/planet.xml
	$(RM) meshes/shuttle.xml
	$(RM) meshes/transporter.xml
	$(RM) textures/particle.tex

clean-recursive: clean-here

data.arx: $(data_arx)
	@$(RM) -f $@
	@echo -e "\n\nData location is set to: $(data_location)\n"
	@echo -e "\nPreparing directory structure ...\n"
	arxtouch $@
	arxtouch $@ 0
	arxmod $@ 0 -n "Root" -t 0 -st 0 -root
	arxtouch $@ 1
	arxmod $@ 1 -n "User Interface" -t 0 -st 0
	arxmod $@ 0 -a "child" 1
	arxtouch $@ 2
	arxmod $@ 2 -n "Meshes" -t 0 -st 0
	arxmod $@ 0 -a "child" 2
	arxtouch $@ 3
	arxmod $@ 3 -n "Ship Classes" -t 0 -st 0
	arxmod $@ 0 -a "child" 3
	arxtouch $@ 4
	arxmod $@ 4 -n "Asset Classes" -t 0 -st 0
	arxmod $@ 0 -a "child" 4
	arxtouch $@ 5
	arxmod $@ 5 -n "Systems" -t 0 -st 0
	arxmod $@ 0 -a "child" 5
	arxtouch $@ 6
	arxmod $@ 6 -n "System Links" -t 0 -st 0
	arxmod $@ 0 -a "child" 6
	arxtouch $@ 7
	arxmod $@ 7 -n "Weapon Classes" -t 0 -st 0
	arxmod $@ 0 -a "child" 7
	arxtouch $@ 8
	arxmod $@ 8 -n "Commodity Classes" -t 0 -st 0
	arxmod $@ 0 -a "child" 8
	arxtouch $@ 9
	arxmod $@ 9 -n "Slot Classes" -t 0 -st 0
	arxmod $@ 0 -a "child" 9
	arxtouch $@ a
	arxmod $@ a -n "Textures" -t 0 -st 0
	arxmod $@ 0 -a "child" a
	arxtouch $@ b
	arxmod $@ b -n "Models" -t 0 -st 0
	arxmod $@ 0 -a "child" b
	@echo -e "\n\nAdding widgets ...\n"
	@echo -e "\nThe widgets will be read in uid order, so assign proper uids so that superior widgets are read before sub widgets.\n"
	arxadd $@ --$(data_location) --uid 103 --name "Credits" --type 1 --subtype 1 --add "/User Interface" .data/label_credits.data
	arxadd $@ --$(data_location) --uid 108 --name "Current System" --type 1 --subtype 1 --add "/User Interface" .data/label_current_system.data
	arxadd $@ --$(data_location) --uid 10A --name "Energy" --type 1 --subtype 1 --add "/User Interface" .data/label_energy.data
	arxadd $@ --$(data_location) --uid 104 --name "Fuel" --type 1 --subtype 1 --add "/User Interface" .data/label_fuel.data
	arxadd $@ --$(data_location) --uid 132 --name "Hull" --type 1 --subtype 1 --add "/User Interface" .data/label_hull.data
	arxadd $@ --$(data_location) --uid 100 --name "Message" --type 1 --subtype 1 --add "/User Interface" .data/label_message.data
	arxadd $@ --$(data_location) --uid 102 --name "System" --type 1 --subtype 1 --add "/User Interface" .data/label_system.data
	arxadd $@ --$(data_location) --uid 107 --name "Target" --type 1 --subtype 1 --add "/User Interface" .data/label_target.data
	arxadd $@ --$(data_location) --uid 101 --name "Time Warp" --type 1 --subtype 1 --add "/User Interface" .data/label_time_warp.data
	arxadd $@ --$(data_location) --uid 106 --name "Mini Map" --type 1 --subtype 0 --add "/User Interface" .data/widget_mini_map.data
	arxadd $@ --$(data_location) --uid 109 --name "Mini Map Display" --type 1 --subtype 2 --add "/User Interface" .data/widget_mini_map_display.data
	arxadd $@ --$(data_location) --uid 105 --name "Scanner" --type 1 --subtype 0 --add "/User Interface" .data/widget_scanner.data
	arxadd $@ --$(data_location) --uid 126 --name "Scanner Display" --type 1 --subtype 3 --add "/User Interface" .data/widget_scanner_display.data
	@echo -e "\n\nAdding slot classes ...\n"
	arxadd $@ --$(data_location) --name "Light Battery" --type 9 --subtype 0 --add "/Slot Classes" .data/slot_class_light_battery.data
	arxadd $@ --$(data_location) --name "Light Weapon" --type 9 --subtype 0 --add "/Slot Classes" .data/slot_class_light_weapon.data
	arxadd $@ --$(data_location) --name "Medium Weapon" --type 9 --subtype 0 --add "/Slot Classes" .data/slot_class_medium_weapon.data
	@echo -e "\n\nAdding weapon classes ...\n"
	arxadd $@ --$(data_location) --name "Light Laser" --type 7 --subtype 0 --add "/Weapon Classes" .data/weapon_class_light_laser.data
	arxadd $@ --$(data_location) --name "Medium Laser" --type 7 --subtype 0 --add "/Weapon Classes" .data/weapon_class_medium_laser.data
	@echo -e "\n\nAdding ship classes ...\n"
	arxadd $@ --$(data_location) --name "Fighter" --type 3 --subtype 0 --add "/Ship Classes" .data/ship_class_fighter.data
	arxadd $@ --$(data_location) --name "Shuttle" --type 3 --subtype 0 --add "/Ship Classes" .data/ship_class_shuttle.data
	arxadd $@ --$(data_location) --name "Transporter" --type 3 --subtype 0 --add "/Ship Classes" .data/ship_class_transporter.data
	@echo -e "\n\nAdding asset classes ...\n"
	arxadd $@ --$(data_location) --name "Equipment" --type 4 --subtype 0 --add "/Asset Classes" .data/asset_class_equipment.data
	arxadd $@ --$(data_location) --name "Food" --type 4 --subtype 0 --add "/Asset Classes" .data/asset_class_food.data
	arxadd $@ --$(data_location) --name "Fuel" --type 4 --subtype 0 --add "/Asset Classes" .data/asset_class_fuel.data
	arxadd $@ --$(data_location) --name "Industrial" --type 4 --subtype 0 --add "/Asset Classes" .data/asset_class_industrial.data
	arxadd $@ --$(data_location) --name "Light Battery" --type 4 --subtype 0 --add "/Asset Classes" .data/asset_class_light_battery.data
	arxadd $@ --$(data_location) --name "Light Laser" --type 4 --subtype 0 --add "/Asset Classes" .data/asset_class_light_laser.data
	arxadd $@ --$(data_location) --name "Luxury Goods" --type 4 --subtype 0 --add "/Asset Classes" .data/asset_class_luxury_goods.data
	arxadd $@ --$(data_location) --name "Medical Supplies" --type 4 --subtype 0 --add "/Asset Classes" .data/asset_class_medical_supplies.data
	arxadd $@ --$(data_location) --name "Medium Laser" --type 4 --subtype 0 --add "/Asset Classes" .data/asset_class_medium_laser.data
	arxadd $@ --$(data_location) --name "Metal" --type 4 --subtype 0 --add "/Asset Classes" .data/asset_class_metal.data
	@echo -e "\n\nAdding commodity classes ...\n"
	arxadd $@ --$(data_location) --name "Equipment" --type 8 --subtype 0 --add "/Commodity Classes" .data/commodity_class_equipment.data
	arxadd $@ --$(data_location) --name "Food" --type 8 --subtype 0 --add "/Commodity Classes" .data/commodity_class_food.data
	arxadd $@ --$(data_location) --name "Fuel" --type 8 --subtype 0 --add "/Commodity Classes" .data/commodity_class_fuel.data
	arxadd $@ --$(data_location) --name "Industrial" --type 8 --subtype 0 --add "/Commodity Classes" .data/commodity_class_industrial.data
	arxadd $@ --$(data_location) --name "Luxury Goods" --type 8 --subtype 0 --add "/Commodity Classes" .data/commodity_class_luxury_goods.data
	arxadd $@ --$(data_location) --name "Medical Supplies" --type 8 --subtype 0 --add "/Commodity Classes" .data/commodity_class_medical_supplies.data
	arxadd $@ --$(data_location) --name "Metal" --type 8 --subtype 0 --add "/Commodity Classes" .data/commodity_class_metal.data
	@echo -e "\n\nAdding systems ...\n"
	arxadd $@ --$(data_location) --name "Alpha Centauri" --type 5 --subtype 0 --add "/Systems" .data/system_alpha_centauri.data
	arxadd $@ --$(data_location) --name "Rigel" --type 5 --subtype 0 --add "/Systems" .data/system_rigel.data
	arxadd $@ --$(data_location) --name "Sol" --type 5 --subtype 0 --subtype 0 --add "/Systems" .data/system_sol.data
	arxadd $@ --$(data_location) --name "Tichel" --type 5 --subtype 0 --add "/Systems" .data/system_tichel.data
	@echo -e "\n\nAdding system links ...\n"
	arxadd $@ --$(data_location) --name "Alpha Centauri / Sol" --type 6 --subtype 0 --add "/System Links" .data/system_link_alpha_centauri_sol.data
	arxadd $@ --$(data_location) --name "Alpha Centauri / Tichel" --type 6 --subtype 0 --add "/System Links" .data/system_link_alpha_centauri_tichel.data
	arxadd $@ --$(data_location) --name "Rigel / Sol" --type 6 --subtype 0 --add "/System Links" .data/system_link_rigel_sol.data
	arxadd $@ --$(data_location) --name "Rigel / Tichel" --type 6 --subtype 0 --add "/System Links" .data/system_link_rigel_tichel.data
	@echo -e "\n\nAdding models ...\n"
	arxadd $@ --$(data_location) --name "Cargo Cube" --type b --subtype 0 --add "/Models" .data/model_cargo_cube.data
	arxadd $@ --$(data_location) --name "Fighter" --type b --subtype 0 --add "/Models" .data/model_fighter.data
	arxadd $@ --$(data_location) --name "Laser Shot" --type b --subtype 0 --add "/Models" .data/model_laser_shot.data
	arxadd $@ --$(data_location) --name "Light Laser" --type b --subtype 0 --add "/Models" .data/model_light_laser.data
	arxadd $@ --$(data_location) --name "Planet" --type b --subtype 0 --add "/Models" .data/model_planet.data
	arxadd $@ --$(data_location) --name "Shuttle" --type b --subtype 0 --add "/Models" .data/model_shuttle.data
	arxadd $@ --$(data_location) --name "Transporter" --type b --subtype 0 --add "/Models" .data/model_transporter.data
	@echo -e "\n\nAdding meshes ...\n"
	arxadd $@ --$(data_location) --name "Cargo Cube" --type 2 --subtype 0 --add "/Meshes" .data/mesh_cargo_cube.data
	arxadd $@ --$(data_location) --name "Fighter Cockpit" --type 2 --subtype 0 --add "/Meshes" .data/mesh_fighter_cockpit.data
	arxadd $@ --$(data_location) --name "Fighter Hull" --type 2 --subtype 0 --add "/Meshes" .data/mesh_fighter_hull.data
	arxadd $@ --$(data_location) --name "Laser Shot" --type 2 --subtype 0 --add "/Meshes" .data/mesh_laser_shot.data
	arxadd $@ --$(data_location) --name "Light Laser" --type 2 --subtype 0 --add "/Meshes" .data/mesh_light_laser.data
	arxadd $@ --$(data_location) --name "Planet" --type 2 --subtype 0 --add "/Meshes" .data/mesh_planet.data
	arxadd $@ --$(data_location) --name "Shuttle Hull" --type 2 --subtype 0 --add "/Meshes" .data/mesh_shuttle_hull.data
	arxadd $@ --$(data_location) --name "Transporter Hull" --type 2 --subtype 0 --add "/Meshes" .data/mesh_transporter_hull.data
	@echo -e "\n\nAdding textures ...\n"
	arxadd $@ --$(data_location) --name "Particle" --type a --subtype 0 --add "/Textures" .data/texture_particle.data

.data:
	mkdir $@

.data/%.data: %.xml $(declarations_xml) $(xml_to_data_py)
	python tools/xml_to_data.py --declarations=tools/declarations.xml --in=$< --out=$@

mesh_%.xml: meshes/%.xml $(littlem_to_xml_xsl)
	xsltproc -output $@ tools/littlem_to_xml.xsl $<

meshes/%.xml: meshes/%.raw $(raw_to_littlem_py)
	python tools/raw_to_littlem.py --in=$< --out=$@ --identifier=$(subst meshes/,,$(subst .xml,,$@)) $(subst $(subst meshes/,,$(subst .xml,,$@)),--smooth,$(findstring $(subst meshes/,,$(subst .xml,,$@)),$(smooth_meshes)))

textures/%.tex: textures/%.vg $(rasterize_py)
	python tools/rasterize.py --in=$< --out=$@ --sub-pixels=3 --dimensions=32x32

.data/texture_particle.data: textures/particle.tex

.PHONY: all all-here all-recursive check-dependencies clean clean-here clean-recursive default
