root = ..

-include $(root)/configuration

default: all

include $(root)/tools/raw_to_littlem/Makefile.include
include $(root)/tools/xml_to_data/Makefile.include
include $(root)/tools/pyarx/Makefile.include

data_arx = \
	Makefile \
	$(all_data)

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
	.data/asset_class_small_generator.data \
	.data/battery_class_light_battery.data \
	.data/commodity_class_equipment.data \
	.data/commodity_class_food.data \
	.data/commodity_class_fuel.data \
	.data/commodity_class_industrial.data \
	.data/commodity_class_luxury_goods.data \
	.data/commodity_class_medical_supplies.data \
	.data/commodity_class_metal.data \
	.data/faction_earth.data \
	.data/faction_pirates.data \
	.data/generator_class_small_generator.data \
	.data/label_credits.data \
	.data/label_energy.data \
	.data/label_fuel.data \
	.data/label_hull.data \
	.data/label_message.data \
	.data/label_system.data \
	.data/label_time_warp.data \
	.data/mesh_cargo_cube.data \
	.data/mesh_fighter_cockpit.data \
	.data/mesh_fighter_faction.data \
	.data/mesh_fighter_hull.data \
	.data/mesh_laser_shot.data \
	.data/mesh_light_laser.data \
	.data/mesh_planet.data \
	.data/mesh_shuttle_faction.data \
	.data/mesh_shuttle_hull.data \
	.data/mesh_transporter_faction.data \
	.data/mesh_transporter_hull.data \
	.data/model_cargo_cube.data \
	.data/model_fighter.data \
	.data/model_laser_shot.data \
	.data/model_light_laser.data \
	.data/model_planet.data \
	.data/model_shuttle.data \
	.data/model_transporter.data \
	.data/setting_key_binding_profile.data \
	.data/setting_window_dimensions.data \
	.data/ship_class_shuttle.data \
	.data/ship_class_transporter.data \
	.data/ship_class_fighter.data \
	.data/slot_class_light_battery.data \
	.data/slot_class_light_weapon.data \
	.data/slot_class_medium_weapon.data \
	.data/slot_class_small_generator.data \
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
	.data/widget_scanner.data \
	.data/savegame_default.data

declarations_xml = \
	tools/declarations.xml

littlem_to_xml_xsl = \
	tools/littlem_to_xml.xsl

all: all-recursive

all-here: check-dependencies $(all_data) data.arx

all-recursive: meshes-all-recursive textures-all-recursive all-here

check-dependencies: raw_to_littlem-check-dependencies xml_to_data-check-dependencies arxadd-check-dependencies arxtouch-check-dependencies
	@if [ x`which python` = x ]; then \
	    echo -e "\nYour system is missing the python interpreter.\nIn order to build the data file you will need to install it on your system.\n"; false; \
	fi
	@if [ x`which xsltproc` = x ]; then \
	    echo -e "\nYour system is missing the xsltproc program.\nTo build the data file you will need to install it on your system.\n"; false; \
	fi

clean: clean-recursive

clean-here:
	$(RM) -r .data
	$(RM) data.arx
	$(RM) mesh_cargo_cube.xml
	$(RM) mesh_fighter.xml
	$(RM) mesh_laser_shot.xml
	$(RM) mesh_light_laser.xml
	$(RM) mesh_planet.xml
	$(RM) mesh_shuttle.xml
	$(RM) mesh_transporter.xml
	$(RM) mesh_transporter_faction.xml

clean-recursive: clean-here meshes-clean-recursive textures-clean-recursive

data.arx: $(data_arx) $(arxtouch_dependencies) $(arxadd_dependencies)
	@$(RM) -f $@
	@echo "\nPreparing directory structure ...\n"
	$(command_arxtouch) $@
	$(command_arxadd) $@ --identifier=0 --name="Root" --type=0 --sub-type=0 --root
	$(command_arxadd) $@ --identifier=1 --name="User Interface" --type=0 --sub-type=0 --relation=0::child
	$(command_arxadd) $@ --identifier=2 --name="Meshes" --type=0 --sub-type=0 --relation=0::child
	$(command_arxadd) $@ --identifier=3 --name="Ship Classes" --type=0 --sub-type=0 --relation=0::child
	$(command_arxadd) $@ --identifier=4 --name="Asset Classes" --type=0 --sub-type=0 --relation=0::child
	$(command_arxadd) $@ --identifier=5 --name="Systems" --type=0 --sub-type=0 --relation=0::child
	$(command_arxadd) $@ --identifier=6 --name="System Links" --type=0 --sub-type=0 --relation=0::child
	$(command_arxadd) $@ --identifier=7 --name="Weapon Classes" --type=0 --sub-type=0 --relation=0::child
	$(command_arxadd) $@ --identifier=8 --name="Commodity Classes" --type=0 --sub-type=0 --relation=0::child
	$(command_arxadd) $@ --identifier=9 --name="Slot Classes" --type=0 --sub-type=0 --relation=0::child
	$(command_arxadd) $@ --identifier=a --name="Textures" --type=0 --sub-type=0 --relation=0::child
	$(command_arxadd) $@ --identifier=b --name="Models" --type=0 --sub-type=0 --relation=0::child
	$(command_arxadd) $@ --identifier=c --name="Battery Classes" --type=0 --sub-type=0 --relation=0::child
	$(command_arxadd) $@ --identifier=d --name="Generator Classes" --type=0 --sub-type=0 --relation=0::child
	$(command_arxadd) $@ --identifier=e --name="Factions" --type=0 --sub-type=0 --relation=0::child
	$(command_arxadd) $@ --identifier=f --name="Settings" --type=0 --sub-type=0 --relation=0::child
	$(command_arxadd) $@ --identifier=10 --name="Savegames" --type=0 --sub-type=0 --relation=0::child
	@echo "\n\nAdding settings ...\n"
	$(command_arxadd) $@ --name="Key Binding Profile" --type=10 --sub-type=0 --relation="/Settings::child" --data-file=.data/setting_key_binding_profile.data
	$(command_arxadd) $@ --name="Window Dimensions" --type=f --sub-type=1 --relation="/Settings::child" --data-file=.data/setting_window_dimensions.data
	@echo "\n\nAdding factions ...\n"
	$(command_arxadd) $@ --name="Earth Faction" --type=e --sub-type=0 --relation="/Factions::child" --data-file=.data/faction_earth.data
	$(command_arxadd) $@ --name="Pirates Faction" --type=e --sub-type=0 --relation="/Factions::child" --data-file=.data/faction_pirates.data
	@echo "\n\nAdding widgets ...\n"
	@echo "\nThe widgets will be read in uid order, so assign proper uids so that superior widgets are read before sub widgets.\n"
	$(command_arxadd) $@ --identifier=103 --name="Credits" --type=1 --sub-type=1 --relation="/User Interface::child" --data-file=.data/label_credits.data
	$(command_arxadd) $@ --identifier=10A --name="Energy" --type=1 --sub-type=1 --relation="/User Interface::child" --data-file=.data/label_energy.data
	$(command_arxadd) $@ --identifier=104 --name="Fuel" --type=1 --sub-type=1 --relation="/User Interface::child" --data-file=.data/label_fuel.data
	$(command_arxadd) $@ --identifier=132 --name="Hull" --type=1 --sub-type=1 --relation="/User Interface::child" --data-file=.data/label_hull.data
	$(command_arxadd) $@ --identifier=100 --name="Message" --type=1 --sub-type=1 --relation="/User Interface::child" --data-file=.data/label_message.data
	$(command_arxadd) $@ --identifier=102 --name="System" --type=1 --sub-type=1 --relation="/User Interface::child" --data-file=.data/label_system.data
	$(command_arxadd) $@ --identifier=101 --name="Time Warp" --type=1 --sub-type=1 --relation="/User Interface::child" --data-file=.data/label_time_warp.data
	$(command_arxadd) $@ --identifier=106 --name="Mini Map" --type=1 --sub-type=0 --relation="/User Interface::child" --data-file=.data/widget_mini_map.data
	$(command_arxadd) $@ --identifier=105 --name="Scanner" --type=1 --sub-type=0 --relation="/User Interface::child" --data-file=.data/widget_scanner.data
	@echo "\n\nAdding slot classes ...\n"
	$(command_arxadd) $@ --name="Light Battery" --type=9 --sub-type=0 --relation="/Slot Classes::child" --data-file=.data/slot_class_light_battery.data
	$(command_arxadd) $@ --name="Light Weapon" --type=9 --sub-type=0 --relation="/Slot Classes::child" --data-file=.data/slot_class_light_weapon.data
	$(command_arxadd) $@ --name="Medium Weapon" --type=9 --sub-type=0 --relation="/Slot Classes::child" --data-file=.data/slot_class_medium_weapon.data
	$(command_arxadd) $@ --name="Small Generator" --type=9 --sub-type=0 --relation="/Slot Classes::child" --data-file=.data/slot_class_small_generator.data
	@echo "\n\nAdding weapon classes ...\n"
	$(command_arxadd) $@ --name="Light Laser" --type=7 --sub-type=0 --relation="/Weapon Classes::child" --data-file=.data/weapon_class_light_laser.data
	$(command_arxadd) $@ --name="Medium Laser" --type=7 --sub-type=0 --relation="/Weapon Classes::child" --data-file=.data/weapon_class_medium_laser.data
	@echo "\n\nAdding battery classes ...\n"
	$(command_arxadd) $@ --name="Light Battery" --type=c --sub-type=0 --relation="/Battery Classes::child" --data-file=.data/battery_class_light_battery.data
	@echo "\n\nAdding generator classes ...\n"
	$(command_arxadd) $@ --name="Small Generator" --type=d --sub-type=0 --relation="/Generator Classes::child" --data-file=.data/generator_class_small_generator.data
	@echo "\n\nAdding ship classes ...\n"
	$(command_arxadd) $@ --name="Fighter" --type=3 --sub-type=0 --relation="/Ship Classes::child" --data-file=.data/ship_class_fighter.data
	$(command_arxadd) $@ --name="Shuttle" --type=3 --sub-type=0 --relation="/Ship Classes::child" --data-file=.data/ship_class_shuttle.data
	$(command_arxadd) $@ --name="Transporter" --type=3 --sub-type=0 --relation="/Ship Classes::child" --data-file=.data/ship_class_transporter.data
	@echo "\n\nAdding asset classes ...\n"
	$(command_arxadd) $@ --name="Equipment" --type=4 --sub-type=0 --relation="/Asset Classes::child" --data-file=.data/asset_class_equipment.data
	$(command_arxadd) $@ --name="Food" --type=4 --sub-type=0 --relation="/Asset Classes::child" --data-file=.data/asset_class_food.data
	$(command_arxadd) $@ --name="Fuel" --type=4 --sub-type=0 --relation="/Asset Classes::child" --data-file=.data/asset_class_fuel.data
	$(command_arxadd) $@ --name="Industrial" --type=4 --sub-type=0 --relation="/Asset Classes::child" --data-file=.data/asset_class_industrial.data
	$(command_arxadd) $@ --name="Light Battery" --type=4 --sub-type=0 --relation="/Asset Classes::child" --data-file=.data/asset_class_light_battery.data
	$(command_arxadd) $@ --name="Light Laser" --type=4 --sub-type=0 --relation="/Asset Classes::child" --data-file=.data/asset_class_light_laser.data
	$(command_arxadd) $@ --name="Luxury Goods" --type=4 --sub-type=0 --relation="/Asset Classes::child" --data-file=.data/asset_class_luxury_goods.data
	$(command_arxadd) $@ --name="Medical Supplies" --type=4 --sub-type=0 --relation="/Asset Classes::child" --data-file=.data/asset_class_medical_supplies.data
	$(command_arxadd) $@ --name="Medium Laser" --type=4 --sub-type=0 --relation="/Asset Classes::child" --data-file=.data/asset_class_medium_laser.data
	$(command_arxadd) $@ --name="Metal" --type=4 --sub-type=0 --relation="/Asset Classes::child" --data-file=.data/asset_class_metal.data
	$(command_arxadd) $@ --name="Small Generator" --type=4 --sub-type=0 --relation="/Asset Classes::child" --data-file=.data/asset_class_small_generator.data
	@echo "\n\nAdding commodity classes ...\n"
	$(command_arxadd) $@ --name="Equipment" --type=8 --sub-type=0 --relation="/Commodity Classes::child" --data-file=.data/commodity_class_equipment.data
	$(command_arxadd) $@ --name="Food" --type=8 --sub-type=0 --relation="/Commodity Classes::child" --data-file=.data/commodity_class_food.data
	$(command_arxadd) $@ --name="Fuel" --type=8 --sub-type=0 --relation="/Commodity Classes::child" --data-file=.data/commodity_class_fuel.data
	$(command_arxadd) $@ --name="Industrial" --type=8 --sub-type=0 --relation="/Commodity Classes::child" --data-file=.data/commodity_class_industrial.data
	$(command_arxadd) $@ --name="Luxury Goods" --type=8 --sub-type=0 --relation="/Commodity Classes::child" --data-file=.data/commodity_class_luxury_goods.data
	$(command_arxadd) $@ --name="Medical Supplies" --type=8 --sub-type=0 --relation="/Commodity Classes::child" --data-file=.data/commodity_class_medical_supplies.data
	$(command_arxadd) $@ --name="Metal" --type=8 --sub-type=0 --relation="/Commodity Classes::child" --data-file=.data/commodity_class_metal.data
	@echo "\n\nAdding systems ...\n"
	$(command_arxadd) $@ --name="Alpha Centauri" --type=5 --sub-type=0 --relation="/Systems::child" --data-file=.data/system_alpha_centauri.data
	$(command_arxadd) $@ --name="Rigel" --type=5 --sub-type=0 --relation="/Systems::child" --data-file=.data/system_rigel.data
	$(command_arxadd) $@ --name="Sol" --type=5 --sub-type=0 --sub-type=0 --relation="/Systems::child" --data-file=.data/system_sol.data
	$(command_arxadd) $@ --name="Tichel" --type=5 --sub-type=0 --relation="/Systems::child" --data-file=.data/system_tichel.data
	@echo "\n\nAdding system links ...\n"
	$(command_arxadd) $@ --name="Alpha Centauri / Sol" --type=6 --sub-type=0 --relation="/System Links::child" --data-file=.data/system_link_alpha_centauri_sol.data
	$(command_arxadd) $@ --name="Alpha Centauri / Tichel" --type=6 --sub-type=0 --relation="/System Links::child" --data-file=.data/system_link_alpha_centauri_tichel.data
	$(command_arxadd) $@ --name="Rigel / Sol" --type=6 --sub-type=0 --relation="/System Links::child" --data-file=.data/system_link_rigel_sol.data
	$(command_arxadd) $@ --name="Rigel / Tichel" --type=6 --sub-type=0 --relation="/System Links::child" --data-file=.data/system_link_rigel_tichel.data
	@echo "\n\nAdding savegames ...\n"
	$(command_arxadd) $@ --name="Default" --type=10 --sub-type=0 --relation="/Savegames::child" --data-file=.data/savegame_default.data
	@echo "\n\nAdding models ...\n"
	$(command_arxadd) $@ --name="Cargo Cube" --type=b --sub-type=0 --relation="/Models::child" --data-file=.data/model_cargo_cube.data
	$(command_arxadd) $@ --name="Fighter" --type=b --sub-type=0 --relation="/Models::child" --data-file=.data/model_fighter.data
	$(command_arxadd) $@ --name="Laser Shot" --type=b --sub-type=0 --relation="/Models::child" --data-file=.data/model_laser_shot.data
	$(command_arxadd) $@ --name="Light Laser" --type=b --sub-type=0 --relation="/Models::child" --data-file=.data/model_light_laser.data
	$(command_arxadd) $@ --name="Planet" --type=b --sub-type=0 --relation="/Models::child" --data-file=.data/model_planet.data
	$(command_arxadd) $@ --name="Shuttle" --type=b --sub-type=0 --relation="/Models::child" --data-file=.data/model_shuttle.data
	$(command_arxadd) $@ --name="Transporter" --type=b --sub-type=0 --relation="/Models::child" --data-file=.data/model_transporter.data
	@echo "\n\nAdding meshes ...\n"
	$(command_arxadd) $@ --name="Cargo Cube" --type=2 --sub-type=0 --relation="/Meshes::child" --data-file=.data/mesh_cargo_cube.data
	$(command_arxadd) $@ --name="Fighter Cockpit" --type=2 --sub-type=0 --relation="/Meshes::child" --data-file=.data/mesh_fighter_cockpit.data
	$(command_arxadd) $@ --name="Fighter Faction" --type=2 --sub-type=0 --relation="/Meshes::child" --data-file=.data/mesh_fighter_faction.data
	$(command_arxadd) $@ --name="Fighter Hull" --type=2 --sub-type=0 --relation="/Meshes::child" --data-file=.data/mesh_fighter_hull.data
	$(command_arxadd) $@ --name="Laser Shot" --type=2 --sub-type=0 --relation="/Meshes::child" --data-file=.data/mesh_laser_shot.data
	$(command_arxadd) $@ --name="Light Laser" --type=2 --sub-type=0 --relation="/Meshes::child" --data-file=.data/mesh_light_laser.data
	$(command_arxadd) $@ --name="Planet" --type=2 --sub-type=0 --relation="/Meshes::child" --data-file=.data/mesh_planet.data
	$(command_arxadd) $@ --name="Shuttle Faction" --type=2 --sub-type=0 --relation="/Meshes::child" --data-file=.data/mesh_shuttle_faction.data
	$(command_arxadd) $@ --name="Shuttle Hull" --type=2 --sub-type=0 --relation="/Meshes::child" --data-file=.data/mesh_shuttle_hull.data
	$(command_arxadd) $@ --name="Transporter Faction" --type=2 --sub-type=0 --relation="/Meshes::child" --data-file=.data/mesh_transporter_faction.data
	$(command_arxadd) $@ --name="Transporter Hull" --type=2 --sub-type=0 --relation="/Meshes::child" --data-file=.data/mesh_transporter_hull.data
	@echo "\n\nAdding textures ...\n"
	$(command_arxadd) $@ --name="Particle" --type=a --sub-type=0 --relation="/Textures::child" --data-file=.data/texture_particle.data

.data:
	mkdir $@

.data/savegame_default.data: savegame_default.xml
	cp -f $< $@

.data/%.data: %.xml $(declarations_xml) $(xml_to_data_dependencies)
	$(command_xml_to_data) --declarations=tools/declarations.xml --in=$< --out=$@

mesh_%.xml: meshes/original/littlem/%.xml $(littlem_to_xml_xsl)
	xsltproc -output $@ tools/littlem_to_xml.xsl $<

mesh_%.xml: meshes/generated/littlem/%.xml $(littlem_to_xml_xsl)
	xsltproc -output $@ tools/littlem_to_xml.xsl $<

meshes-all-recursive:
	@$(MAKE) -C meshes all-recursive

textures-all-recursive:
	@$(MAKE) -C textures all-recursive

meshes-clean-recursive:
	@$(MAKE) -C meshes clean-recursive

textures-clean-recursive:
	@$(MAKE) -C textures clean-recursive

.data/texture_particle.data: textures/generated/particle.tex

.PHONY: all all-here all-recursive check-dependencies clean clean-here clean-recursive default meshes-all-recursive meshes-clean-recursive textures-all-recursive textures-clean-recursive
