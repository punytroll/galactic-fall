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
	.data/asset_class_fighter.data \
	.data/asset_class_food.data \
	.data/asset_class_fuel.data \
	.data/asset_class_industrial.data \
	.data/asset_class_light_battery.data \
	.data/asset_class_light_laser.data \
	.data/asset_class_light_laser_turret.data \
	.data/asset_class_luxury_goods.data \
	.data/asset_class_medical_supplies.data \
	.data/asset_class_medium_laser.data \
	.data/asset_class_metal.data \
	.data/asset_class_shuttle.data \
	.data/asset_class_small_generator.data \
	.data/asset_class_transporter.data \
	.data/battery_class_light_battery.data \
	.data/commodity_class_equipment.data \
	.data/commodity_class_food.data \
	.data/commodity_class_fuel.data \
	.data/commodity_class_industrial.data \
	.data/commodity_class_luxury_goods.data \
	.data/commodity_class_medical_supplies.data \
	.data/commodity_class_metal.data \
	.data/faction_earth.data \
	.data/faction_neutral.data \
	.data/faction_pirates.data \
	.data/fragment_shader_default.data \
	.data/fragment_shader_no_lighting.data \
	.data/generator_class_small_generator.data \
	.data/mesh_cargo_cube.data \
	.data/mesh_fighter_cockpit.data \
	.data/mesh_fighter_faction.data \
	.data/mesh_fighter_hull.data \
	.data/mesh_laser_shot.data \
	.data/mesh_light_battery.data \
	.data/mesh_light_laser.data \
	.data/mesh_light_laser_turret_base.data \
	.data/mesh_light_laser_turret_gun.data \
	.data/mesh_planet.data \
	.data/mesh_shuttle_faction.data \
	.data/mesh_shuttle_hull.data \
	.data/mesh_transporter_faction.data \
	.data/mesh_transporter_hull.data \
	.data/model_cargo_cube.data \
	.data/model_fighter.data \
	.data/model_laser_shot.data \
	.data/model_light_battery.data \
	.data/model_light_laser.data \
	.data/model_light_laser_turret.data \
	.data/model_planet.data \
	.data/model_shuttle.data \
	.data/model_transporter.data \
	.data/program_default.data \
	.data/program_no_lighting.data \
	.data/scenario_new_game.data \
	.data/scenario_rich_game.data \
	.data/scenario_sol_spectator.data \
	.data/setting_key_binding_profile.data \
	.data/setting_window_dimensions.data \
	.data/ship_class_fighter.data \
	.data/ship_class_shuttle.data \
	.data/ship_class_transporter.data \
	.data/slot_class_light_battery.data \
	.data/slot_class_light_turret.data \
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
	.data/texture_font.data \
	.data/texture_particle.data \
	.data/turret_class_light_laser.data \
	.data/vertex_shader_default.data \
	.data/vertex_shader_no_lighting.data \
	.data/weapon_class_light_laser.data \
	.data/weapon_class_medium_laser.data

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
	$(command_arxadd) $@ --name="Root" --type=0 --sub-type=0 --root
	$(command_arxadd) $@ --name="User Interface" --type=0 --sub-type=0 --relation="::child"
	$(command_arxadd) $@ --name="Meshes" --type=0 --sub-type=0 --relation="::child"
	$(command_arxadd) $@ --name="Ship Classes" --type=0 --sub-type=0 --relation="::child"
	$(command_arxadd) $@ --name="Asset Classes" --type=0 --sub-type=0 --relation="::child"
	$(command_arxadd) $@ --name="Systems" --type=0 --sub-type=0 --relation="::child"
	$(command_arxadd) $@ --name="System Links" --type=0 --sub-type=0 --relation="::child"
	$(command_arxadd) $@ --name="Weapon Classes" --type=0 --sub-type=0 --relation="::child"
	$(command_arxadd) $@ --name="Turret Classes" --type=0 --sub-type=0 --relation="::child"
	$(command_arxadd) $@ --name="Commodity Classes" --type=0 --sub-type=0 --relation="::child"
	$(command_arxadd) $@ --name="Slot Classes" --type=0 --sub-type=0 --relation="::child"
	$(command_arxadd) $@ --name="Textures" --type=0 --sub-type=0 --relation="::child"
	$(command_arxadd) $@ --name="Models" --type=0 --sub-type=0 --relation="::child"
	$(command_arxadd) $@ --name="Battery Classes" --type=0 --sub-type=0 --relation="::child"
	$(command_arxadd) $@ --name="Generator Classes" --type=0 --sub-type=0 --relation="::child"
	$(command_arxadd) $@ --name="Factions" --type=0 --sub-type=0 --relation="::child"
	$(command_arxadd) $@ --name="Settings" --type=0 --sub-type=0 --relation="::child"
	$(command_arxadd) $@ --name="Scenarios" --type=0 --sub-type=0 --relation="::child"
	$(command_arxadd) $@ --name="Shaders" --type=0 --sub-type=0 --relation="::child"
	$(command_arxadd) $@ --name="Programs" --type=0 --sub-type=0 --relation="::child"
	@echo "\n\nAdding settings ...\n"
	$(command_arxadd) $@ --name="Key Binding Profile" --type=10 --sub-type=0 --relation="/Settings::child" --data-file=.data/setting_key_binding_profile.data
	$(command_arxadd) $@ --name="Window Dimensions" --type=f --sub-type=0 --relation="/Settings::child" --data-file=.data/setting_window_dimensions.data
	@echo "\n\nAdding factions ...\n"
	$(command_arxadd) $@ --name="Earth Faction" --type=e --sub-type=0 --relation="/Factions::child" --data-file=.data/faction_earth.data
	$(command_arxadd) $@ --name="Neutral Faction" --type=e --sub-type=0 --relation="/Factions::child" --data-file=.data/faction_neutral.data
	$(command_arxadd) $@ --name="Pirates Faction" --type=e --sub-type=0 --relation="/Factions::child" --data-file=.data/faction_pirates.data
	@echo "\n\nAdding slot classes ...\n"
	$(command_arxadd) $@ --name="Light Battery" --type=9 --sub-type=0 --relation="/Slot Classes::child" --data-file=.data/slot_class_light_battery.data
	$(command_arxadd) $@ --name="Light Turret" --type=9 --sub-type=0 --relation="/Slot Classes::child" --data-file=.data/slot_class_light_turret.data
	$(command_arxadd) $@ --name="Light Weapon" --type=9 --sub-type=0 --relation="/Slot Classes::child" --data-file=.data/slot_class_light_weapon.data
	$(command_arxadd) $@ --name="Medium Weapon" --type=9 --sub-type=0 --relation="/Slot Classes::child" --data-file=.data/slot_class_medium_weapon.data
	$(command_arxadd) $@ --name="Small Generator" --type=9 --sub-type=0 --relation="/Slot Classes::child" --data-file=.data/slot_class_small_generator.data
	@echo "\n\nAdding weapon classes ...\n"
	$(command_arxadd) $@ --name="Light Laser" --type=7 --sub-type=0 --relation="/Weapon Classes::child" --data-file=.data/weapon_class_light_laser.data
	$(command_arxadd) $@ --name="Medium Laser" --type=7 --sub-type=0 --relation="/Weapon Classes::child" --data-file=.data/weapon_class_medium_laser.data
	@echo "\n\nAdding turret classes ...\n"
	$(command_arxadd) $@ --name="Light Laser Turret" --type=16 --sub-type=0 --relation="/Turret Classes::child" --data-file=.data/turret_class_light_laser.data
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
	$(command_arxadd) $@ --name="Fighter" --type=4 --sub-type=0 --relation="/Asset Classes::child" --data-file=.data/asset_class_fighter.data
	$(command_arxadd) $@ --name="Food" --type=4 --sub-type=0 --relation="/Asset Classes::child" --data-file=.data/asset_class_food.data
	$(command_arxadd) $@ --name="Fuel" --type=4 --sub-type=0 --relation="/Asset Classes::child" --data-file=.data/asset_class_fuel.data
	$(command_arxadd) $@ --name="Industrial" --type=4 --sub-type=0 --relation="/Asset Classes::child" --data-file=.data/asset_class_industrial.data
	$(command_arxadd) $@ --name="Light Battery" --type=4 --sub-type=0 --relation="/Asset Classes::child" --data-file=.data/asset_class_light_battery.data
	$(command_arxadd) $@ --name="Light Laser" --type=4 --sub-type=0 --relation="/Asset Classes::child" --data-file=.data/asset_class_light_laser.data
	$(command_arxadd) $@ --name="Light Laser Turret" --type=4 --sub-type=0 --relation="/Asset Classes::child" --data-file=.data/asset_class_light_laser_turret.data
	$(command_arxadd) $@ --name="Luxury Goods" --type=4 --sub-type=0 --relation="/Asset Classes::child" --data-file=.data/asset_class_luxury_goods.data
	$(command_arxadd) $@ --name="Medical Supplies" --type=4 --sub-type=0 --relation="/Asset Classes::child" --data-file=.data/asset_class_medical_supplies.data
	$(command_arxadd) $@ --name="Medium Laser" --type=4 --sub-type=0 --relation="/Asset Classes::child" --data-file=.data/asset_class_medium_laser.data
	$(command_arxadd) $@ --name="Metal" --type=4 --sub-type=0 --relation="/Asset Classes::child" --data-file=.data/asset_class_metal.data
	$(command_arxadd) $@ --name="Shuttle" --type=4 --sub-type=0 --relation="/Asset Classes::child" --data-file=.data/asset_class_shuttle.data
	$(command_arxadd) $@ --name="Small Generator" --type=4 --sub-type=0 --relation="/Asset Classes::child" --data-file=.data/asset_class_small_generator.data
	$(command_arxadd) $@ --name="Transporter" --type=4 --sub-type=0 --relation="/Asset Classes::child" --data-file=.data/asset_class_transporter.data
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
	@echo "\n\nAdding models ...\n"
	$(command_arxadd) $@ --name="Cargo Cube" --type=b --sub-type=0 --relation="/Models::child" --data-file=.data/model_cargo_cube.data
	$(command_arxadd) $@ --name="Fighter" --type=b --sub-type=0 --relation="/Models::child" --data-file=.data/model_fighter.data
	$(command_arxadd) $@ --name="Laser Shot" --type=b --sub-type=0 --relation="/Models::child" --data-file=.data/model_laser_shot.data
	$(command_arxadd) $@ --name="Light Battery" --type=b --sub-type=0 --relation="/Models::child" --data-file=.data/model_light_battery.data
	$(command_arxadd) $@ --name="Light Laser" --type=b --sub-type=0 --relation="/Models::child" --data-file=.data/model_light_laser.data
	$(command_arxadd) $@ --name="Light Laser Turret" --type=b --sub-type=0 --relation="/Models::child" --data-file=.data/model_light_laser_turret.data
	$(command_arxadd) $@ --name="Planet" --type=b --sub-type=0 --relation="/Models::child" --data-file=.data/model_planet.data
	$(command_arxadd) $@ --name="Shuttle" --type=b --sub-type=0 --relation="/Models::child" --data-file=.data/model_shuttle.data
	$(command_arxadd) $@ --name="Transporter" --type=b --sub-type=0 --relation="/Models::child" --data-file=.data/model_transporter.data
	@echo "\n\nAdding meshes ...\n"
	$(command_arxadd) $@ --name="Cargo Cube" --type=2 --sub-type=0 --relation="/Meshes::child" --data-file=.data/mesh_cargo_cube.data
	$(command_arxadd) $@ --name="Fighter Cockpit" --type=2 --sub-type=0 --relation="/Meshes::child" --data-file=.data/mesh_fighter_cockpit.data
	$(command_arxadd) $@ --name="Fighter Faction" --type=2 --sub-type=0 --relation="/Meshes::child" --data-file=.data/mesh_fighter_faction.data
	$(command_arxadd) $@ --name="Fighter Hull" --type=2 --sub-type=0 --relation="/Meshes::child" --data-file=.data/mesh_fighter_hull.data
	$(command_arxadd) $@ --name="Laser Shot" --type=2 --sub-type=0 --relation="/Meshes::child" --data-file=.data/mesh_laser_shot.data
	$(command_arxadd) $@ --name="Light Battery" --type=2 --sub-type=0 --relation="/Meshes::child" --data-file=.data/mesh_light_battery.data
	$(command_arxadd) $@ --name="Light Laser" --type=2 --sub-type=0 --relation="/Meshes::child" --data-file=.data/mesh_light_laser.data
	$(command_arxadd) $@ --name="Light Laser Turret Base" --type=2 --sub-type=0 --relation="/Meshes::child" --data-file=.data/mesh_light_laser_turret_base.data
	$(command_arxadd) $@ --name="Light Laser Turret Gun" --type=2 --sub-type=0 --relation="/Meshes::child" --data-file=.data/mesh_light_laser_turret_gun.data
	$(command_arxadd) $@ --name="Planet" --type=2 --sub-type=0 --relation="/Meshes::child" --data-file=.data/mesh_planet.data
	$(command_arxadd) $@ --name="Shuttle Faction" --type=2 --sub-type=0 --relation="/Meshes::child" --data-file=.data/mesh_shuttle_faction.data
	$(command_arxadd) $@ --name="Shuttle Hull" --type=2 --sub-type=0 --relation="/Meshes::child" --data-file=.data/mesh_shuttle_hull.data
	$(command_arxadd) $@ --name="Transporter Faction" --type=2 --sub-type=0 --relation="/Meshes::child" --data-file=.data/mesh_transporter_faction.data
	$(command_arxadd) $@ --name="Transporter Hull" --type=2 --sub-type=0 --relation="/Meshes::child" --data-file=.data/mesh_transporter_hull.data
	@echo "\n\nAdding textures ...\n"
	$(command_arxadd) $@ --name="Font" --type=a --sub-type=0 --relation="/Textures::child" --data-file=.data/texture_font.data
	$(command_arxadd) $@ --name="Particle" --type=a --sub-type=0 --relation="/Textures::child" --data-file=.data/texture_particle.data
	@echo "\n\nAdding scenarios ...\n"
	$(command_arxadd) $@ --name="New Game" --type=11 --sub-type=0 --relation="/Scenarios::child" --data-file=.data/scenario_new_game.data
	$(command_arxadd) $@ --name="Rich Game" --type=11 --sub-type=0 --relation="/Scenarios::child" --data-file=.data/scenario_rich_game.data
	$(command_arxadd) $@ --name="Sol Spectator" --type=11 --sub-type=0 --relation="/Scenarios::child" --data-file=.data/scenario_sol_spectator.data
	@echo "\n\nAdding shaders ...\n"
	$(command_arxadd) $@ --name="Default Fragment Shader" --type=13 --sub-type=1 --relation="/Shaders::child" --data-file=.data/fragment_shader_default.data
	$(command_arxadd) $@ --name="Default Vertex Shader" --type=13 --sub-type=0 --relation="/Shaders::child" --data-file=.data/vertex_shader_default.data
	$(command_arxadd) $@ --name="No Lighting Fragment Shader" --type=13 --sub-type=1 --relation="/Shaders::child" --data-file=.data/fragment_shader_no_lighting.data
	$(command_arxadd) $@ --name="No Lighting Vertex Shader" --type=13 --sub-type=0 --relation="/Shaders::child" --data-file=.data/vertex_shader_no_lighting.data
	@echo "\n\nAdding programs ...\n"
	$(command_arxadd) $@ --name="Default Program" --type=14 --sub-type=0 --relation="/Programs::child" --data-file=.data/program_default.data
	$(command_arxadd) $@ --name="No Lighting Program" --type=14 --sub-type=0 --relation="/Programs::child" --data-file=.data/program_no_lighting.data

.data:
	mkdir $@

.data/%.data: %.xml $(declarations_xml) $(xml_to_data_dependencies)
	$(command_xml_to_data) --declarations=tools/declarations.xml --in=$< --out=$@

mesh_%.xml: meshes/original/littlem/%.xml $(littlem_to_xml_xsl)
	xsltproc -output $@ tools/littlem_to_xml.xsl $<

mesh_%.xml: meshes/generated/littlem/%.xml $(littlem_to_xml_xsl)
	xsltproc -output $@ tools/littlem_to_xml.xsl $<

meshes-all-recursive:
	@$(MAKE) -C meshes all-recursive

.data/fragment_shader_default.data: shaders/fragment_shader_default

.data/fragment_shader_no_lighting.data: shaders/fragment_shader_no_lighting

.data/scenario_new_game.data: savegame_new_game.xml

.data/scenario_sol_spectator.data: savegame_sol_spectator.xml

.data/vertex_shader_default.data: shaders/vertex_shader_default

.data/vertex_shader_no_lighting.data: shaders/vertex_shader_no_lighting

textures-all-recursive:
	@$(MAKE) -C textures all-recursive

meshes-clean-recursive:
	@$(MAKE) -C meshes clean-recursive

textures-clean-recursive:
	@$(MAKE) -C textures clean-recursive

.PHONY: all all-here all-recursive check-dependencies clean clean-here clean-recursive default meshes-all-recursive meshes-clean-recursive textures-all-recursive textures-clean-recursive
