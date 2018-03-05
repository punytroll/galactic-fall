root = ..

default: all

include $(root)/tools/littlem_to_xml/Makefile.include
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
	.data/fragment_shader_default.data \
	.data/fragment_shader_flat.data \
	.data/fragment_shader_font.data \
	.data/fragment_shader_no_lighting.data \
	.data/fragment_shader_particle.data \
	.data/fragment_shader_ui_texture.data \
	.data/fragment_shader_widget.data \
	.data/galaxies \
	.data/galaxies/milky_way \
	.data/galaxies/milky_way/faction_earth.data \
	.data/galaxies/milky_way/faction_neutral.data \
	.data/galaxies/milky_way/faction_pirates.data \
	.data/galaxies/milky_way/galaxy.data \
	.data/galaxies/milky_way/planet_earth.data \
	.data/galaxies/milky_way/planet_hephant.data \
	.data/galaxies/milky_way/planet_mars.data \
	.data/galaxies/milky_way/planet_moon.data \
	.data/galaxies/milky_way/planet_rigel_guardian.data \
	.data/galaxies/milky_way/planet_tau_prime.data \
	.data/galaxies/milky_way/star_alpha_centauri.data \
	.data/galaxies/milky_way/star_rigel.data \
	.data/galaxies/milky_way/star_sol.data \
	.data/galaxies/milky_way/star_tichel.data \
	.data/galaxies/milky_way/system_alpha_centauri.data \
	.data/galaxies/milky_way/system_rigel.data \
	.data/galaxies/milky_way/system_sol.data \
	.data/galaxies/milky_way/system_tichel.data \
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
	.data/mesh_small_generator.data \
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
	.data/model_small_generator.data \
	.data/model_transporter.data \
	.data/program_default.data \
	.data/program_flat.data \
	.data/program_font.data \
	.data/program_font_without_clipping.data \
	.data/program_no_lighting.data \
	.data/program_particle.data \
	.data/program_ui_texture.data \
	.data/program_widget.data \
	.data/scenario_new_game.data \
	.data/scenario_rich_game.data \
	.data/scenario_rigel_spectator.data \
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
	.data/texture_font.data \
	.data/texture_particle.data \
	.data/turret_class_light_laser.data \
	.data/vertex_shader_default.data \
	.data/vertex_shader_flat.data \
	.data/vertex_shader_font.data \
	.data/vertex_shader_font_without_clipping.data \
	.data/vertex_shader_no_lighting.data \
	.data/vertex_shader_particle.data \
	.data/vertex_shader_ui_texture.data \
	.data/vertex_shader_widget.data \
	.data/weapon_class_light_laser.data \
	.data/weapon_class_medium_laser.data

declarations_xml = \
	tools/declarations.xml

littlem_to_xml_xsl = \
	tools/littlem_to_xml.xsl

all: all-recursive

all-here: check-dependencies $(all_data) data.arx

all-recursive: textures-all-recursive all-here

check-dependencies: littlem_to_xml-check-dependencies xml_to_data-check-dependencies arxadd-check-dependencies arxtouch-check-dependencies

clean: clean-recursive

clean-here:
	$(RM) -r .data
	$(RM) data.arx
	$(RM) mesh_cargo_cube.xml
	$(RM) mesh_fighter_cockpit.xml
	$(RM) mesh_fighter_faction.xml
	$(RM) mesh_fighter_hull.xml
	$(RM) mesh_laser_shot.xml
	$(RM) mesh_light_battery.xml
	$(RM) mesh_light_laser_turret_base.xml
	$(RM) mesh_light_laser_turret_gun.xml
	$(RM) mesh_light_laser.xml
	$(RM) mesh_planet.xml
	$(RM) mesh_shuttle_faction.xml
	$(RM) mesh_shuttle_hull.xml
	$(RM) mesh_small_generator.xml
	$(RM) mesh_transporter_faction.xml
	$(RM) mesh_transporter_hull.xml

clean-recursive: clean-here textures-clean-recursive

data.arx: $(data_arx) $(arxtouch_dependencies) $(arxadd_dependencies)
	@$(RM) -f $@
	@echo "\nPreparing directory structure ...\n"
	$(command_arxtouch) $@
	$(command_arxadd) $@ --name="Root" --type=0 --sub-type=0 --root
	$(command_arxadd) $@ --name="User Interface" --type=0 --sub-type=0 --relation="::child"
	$(command_arxadd) $@ --name="Meshes" --type=0 --sub-type=0 --relation="::child"
	$(command_arxadd) $@ --name="Ship Classes" --type=0 --sub-type=0 --relation="::child"
	$(command_arxadd) $@ --name="Asset Classes" --type=0 --sub-type=0 --relation="::child"
	$(command_arxadd) $@ --name="Weapon Classes" --type=0 --sub-type=0 --relation="::child"
	$(command_arxadd) $@ --name="Turret Classes" --type=0 --sub-type=0 --relation="::child"
	$(command_arxadd) $@ --name="Commodity Classes" --type=0 --sub-type=0 --relation="::child"
	$(command_arxadd) $@ --name="Slot Classes" --type=0 --sub-type=0 --relation="::child"
	$(command_arxadd) $@ --name="Textures" --type=0 --sub-type=0 --relation="::child"
	$(command_arxadd) $@ --name="Models" --type=0 --sub-type=0 --relation="::child"
	$(command_arxadd) $@ --name="Battery Classes" --type=0 --sub-type=0 --relation="::child"
	$(command_arxadd) $@ --name="Generator Classes" --type=0 --sub-type=0 --relation="::child"
	$(command_arxadd) $@ --name="Settings" --type=0 --sub-type=0 --relation="::child"
	$(command_arxadd) $@ --name="Scenarios" --type=0 --sub-type=0 --relation="::child"
	$(command_arxadd) $@ --name="Shaders" --type=0 --sub-type=0 --relation="::child"
	$(command_arxadd) $@ --name="Programs" --type=0 --sub-type=0 --relation="::child"
	$(command_arxadd) $@ --name="Galaxies" --type=0 --sub-type=0 --relation="::child"
	@echo "\n\nAdding settings ...\n"
	$(command_arxadd) $@ --name="Key Binding Profile" --type=10 --sub-type=0 --relation="/Settings::child" --data-file=.data/setting_key_binding_profile.data
	$(command_arxadd) $@ --name="Window Dimensions" --type=f --sub-type=0 --relation="/Settings::child" --data-file=.data/setting_window_dimensions.data
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
	@echo "\n\nAdding models ...\n"
	$(command_arxadd) $@ --name="Cargo Cube" --type=b --sub-type=0 --relation="/Models::child" --data-file=.data/model_cargo_cube.data
	$(command_arxadd) $@ --name="Fighter" --type=b --sub-type=0 --relation="/Models::child" --data-file=.data/model_fighter.data
	$(command_arxadd) $@ --name="Laser Shot" --type=b --sub-type=0 --relation="/Models::child" --data-file=.data/model_laser_shot.data
	$(command_arxadd) $@ --name="Light Battery" --type=b --sub-type=0 --relation="/Models::child" --data-file=.data/model_light_battery.data
	$(command_arxadd) $@ --name="Light Laser" --type=b --sub-type=0 --relation="/Models::child" --data-file=.data/model_light_laser.data
	$(command_arxadd) $@ --name="Light Laser Turret" --type=b --sub-type=0 --relation="/Models::child" --data-file=.data/model_light_laser_turret.data
	$(command_arxadd) $@ --name="Planet" --type=b --sub-type=0 --relation="/Models::child" --data-file=.data/model_planet.data
	$(command_arxadd) $@ --name="Shuttle" --type=b --sub-type=0 --relation="/Models::child" --data-file=.data/model_shuttle.data
	$(command_arxadd) $@ --name="Small Generator" --type=b --sub-type=0 --relation="/Models::child" --data-file=.data/model_small_generator.data
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
	$(command_arxadd) $@ --name="Small Generator" --type=2 --sub-type=0 --relation="/Meshes::child" --data-file=.data/mesh_small_generator.data
	$(command_arxadd) $@ --name="Transporter Faction" --type=2 --sub-type=0 --relation="/Meshes::child" --data-file=.data/mesh_transporter_faction.data
	$(command_arxadd) $@ --name="Transporter Hull" --type=2 --sub-type=0 --relation="/Meshes::child" --data-file=.data/mesh_transporter_hull.data
	@echo "\n\nAdding textures ...\n"
	$(command_arxadd) $@ --name="Font" --type=a --sub-type=0 --relation="/Textures::child" --data-file=.data/texture_font.data
	$(command_arxadd) $@ --name="Particle" --type=a --sub-type=0 --relation="/Textures::child" --data-file=.data/texture_particle.data
	@echo "\n\nAdding scenarios ...\n"
	$(command_arxadd) $@ --name="New Game" --type=11 --sub-type=0 --relation="/Scenarios::child" --data-file=.data/scenario_new_game.data
	$(command_arxadd) $@ --name="Rich Game" --type=11 --sub-type=0 --relation="/Scenarios::child" --data-file=.data/scenario_rich_game.data
	$(command_arxadd) $@ --name="Rigel Spectator" --type=11 --sub-type=0 --relation="/Scenarios::child" --data-file=.data/scenario_rigel_spectator.data
	@echo "\n\nAdding shaders ...\n"
	$(command_arxadd) $@ --name="Default Fragment Shader" --type=13 --sub-type=1 --relation="/Shaders::child" --data-file=.data/fragment_shader_default.data
	$(command_arxadd) $@ --name="Default Vertex Shader" --type=13 --sub-type=0 --relation="/Shaders::child" --data-file=.data/vertex_shader_default.data
	$(command_arxadd) $@ --name="Flat Fragment Shader" --type=13 --sub-type=1 --relation="/Shaders::child" --data-file=.data/fragment_shader_flat.data
	$(command_arxadd) $@ --name="Flat Vertex Shader" --type=13 --sub-type=0 --relation="/Shaders::child" --data-file=.data/vertex_shader_flat.data
	$(command_arxadd) $@ --name="Font Fragment Shader" --type=13 --sub-type=1 --relation="/Shaders::child" --data-file=.data/fragment_shader_font.data
	$(command_arxadd) $@ --name="Font Vertex Shader" --type=13 --sub-type=0 --relation="/Shaders::child" --data-file=.data/vertex_shader_font.data
	$(command_arxadd) $@ --name="Font Without Clipping Vertex Shader" --type=13 --sub-type=0 --relation="/Shaders::child" --data-file=.data/vertex_shader_font_without_clipping.data
	$(command_arxadd) $@ --name="No Lighting Fragment Shader" --type=13 --sub-type=1 --relation="/Shaders::child" --data-file=.data/fragment_shader_no_lighting.data
	$(command_arxadd) $@ --name="No Lighting Vertex Shader" --type=13 --sub-type=0 --relation="/Shaders::child" --data-file=.data/vertex_shader_no_lighting.data
	$(command_arxadd) $@ --name="Particle Fragment Shader" --type=13 --sub-type=1 --relation="/Shaders::child" --data-file=.data/fragment_shader_particle.data
	$(command_arxadd) $@ --name="Particle Vertex Shader" --type=13 --sub-type=0 --relation="/Shaders::child" --data-file=.data/vertex_shader_particle.data
	$(command_arxadd) $@ --name="UI Texture Fragment Shader" --type=13 --sub-type=1 --relation="/Shaders::child" --data-file=.data/fragment_shader_ui_texture.data
	$(command_arxadd) $@ --name="UI Texture Vertex Shader" --type=13 --sub-type=0 --relation="/Shaders::child" --data-file=.data/vertex_shader_ui_texture.data
	$(command_arxadd) $@ --name="Widget Fragment Shader" --type=13 --sub-type=1 --relation="/Shaders::child" --data-file=.data/fragment_shader_widget.data
	$(command_arxadd) $@ --name="Widget Vertex Shader" --type=13 --sub-type=0 --relation="/Shaders::child" --data-file=.data/vertex_shader_widget.data
	@echo "\n\nAdding programs ...\n"
	$(command_arxadd) $@ --name="Default Program" --type=14 --sub-type=0 --relation="/Programs::child" --data-file=.data/program_default.data
	$(command_arxadd) $@ --name="Flat Program" --type=14 --sub-type=0 --relation="/Programs::child" --data-file=.data/program_flat.data
	$(command_arxadd) $@ --name="Font Program" --type=14 --sub-type=0 --relation="/Programs::child" --data-file=.data/program_font.data
	$(command_arxadd) $@ --name="Font Without Clipping Program" --type=14 --sub-type=0 --relation="/Programs::child" --data-file=.data/program_font_without_clipping.data
	$(command_arxadd) $@ --name="No Lighting Program" --type=14 --sub-type=0 --relation="/Programs::child" --data-file=.data/program_no_lighting.data
	$(command_arxadd) $@ --name="Particle Program" --type=14 --sub-type=0 --relation="/Programs::child" --data-file=.data/program_particle.data
	$(command_arxadd) $@ --name="UI Texture Program" --type=14 --sub-type=0 --relation="/Programs::child" --data-file=.data/program_ui_texture.data
	$(command_arxadd) $@ --name="Widget Program" --type=14 --sub-type=0 --relation="/Programs::child" --data-file=.data/program_widget.data
	@echo "\n\nAdding \"Milky Way\" galaxy ...\n"
	$(command_arxadd) $@ --name="Milky Way" --type=17 --sub-type=0 --relation="/Galaxies::child" --data-file=.data/galaxies/milky_way/galaxy.data
	$(command_arxadd) $@ --name="Earth" --type=e --sub-type=0 --relation="/Galaxies/Milky Way::factions" --data-file=.data/galaxies/milky_way/faction_earth.data
	$(command_arxadd) $@ --name="Neutral" --type=e --sub-type=0 --relation="/Galaxies/Milky Way::factions" --data-file=.data/galaxies/milky_way/faction_neutral.data
	$(command_arxadd) $@ --name="Pirates" --type=e --sub-type=0 --relation="/Galaxies/Milky Way::factions" --data-file=.data/galaxies/milky_way/faction_pirates.data
	$(command_arxadd) $@ --name="Alpha Centauri" --type=5 --sub-type=0 --relation="/Galaxies/Milky Way::systems" --data-file=.data/galaxies/milky_way/system_alpha_centauri.data
	$(command_arxadd) $@ --name="Alpha Centauri" --type=18 --sub-type=0 --relation="/Galaxies/Milky Way::systems/Alpha Centauri::stars" --data-file=.data/galaxies/milky_way/star_alpha_centauri.data
	$(command_arxadd) $@ --name="Hephant" --type=19 --sub-type=0 --relation="/Galaxies/Milky Way::systems/Alpha Centauri::planets" --data-file=.data/galaxies/milky_way/planet_hephant.data
	$(command_arxadd) $@ --name="Tau Prime" --type=19 --sub-type=0 --relation="/Galaxies/Milky Way::systems/Alpha Centauri::planets" --data-file=.data/galaxies/milky_way/planet_tau_prime.data
	$(command_arxadd) $@ --name="Rigel" --type=5 --sub-type=0 --relation="/Galaxies/Milky Way::systems" --data-file=.data/galaxies/milky_way/system_rigel.data
	$(command_arxadd) $@ --name="Rigel" --type=18 --sub-type=0 --relation="/Galaxies/Milky Way::systems/Rigel::stars" --data-file=.data/galaxies/milky_way/star_rigel.data
	$(command_arxadd) $@ --name="Rigel Guardian" --type=19 --sub-type=0 --relation="/Galaxies/Milky Way::systems/Rigel::planets" --data-file=.data/galaxies/milky_way/planet_rigel_guardian.data
	$(command_arxadd) $@ --name="Sol" --type=5 --sub-type=0 --relation="/Galaxies/Milky Way::systems" --data-file=.data/galaxies/milky_way/system_sol.data
	$(command_arxadd) $@ --name="Sol" --type=18 --sub-type=0 --relation="/Galaxies/Milky Way::systems/Sol::stars" --data-file=.data/galaxies/milky_way/star_sol.data
	$(command_arxadd) $@ --name="Earth" --type=19 --sub-type=0 --relation="/Galaxies/Milky Way::systems/Sol::planets" --data-file=.data/galaxies/milky_way/planet_earth.data
	$(command_arxadd) $@ --name="Mars" --type=19 --sub-type=0 --relation="/Galaxies/Milky Way::systems/Sol::planets" --data-file=.data/galaxies/milky_way/planet_mars.data
	$(command_arxadd) $@ --name="Moon" --type=19 --sub-type=0 --relation="/Galaxies/Milky Way::systems/Sol::planets" --data-file=.data/galaxies/milky_way/planet_moon.data
	$(command_arxadd) $@ --name="Tichel" --type=5 --sub-type=0 --relation="/Galaxies/Milky Way::systems" --data-file=.data/galaxies/milky_way/system_tichel.data
	$(command_arxadd) $@ --name="Tichel" --type=18 --sub-type=0 --relation="/Galaxies/Milky Way::systems/Tichel::stars" --data-file=.data/galaxies/milky_way/star_tichel.data
	$(command_arxadd) $@ --path="/Galaxies/Milky Way::systems/Alpha Centauri" --relation="/Galaxies/Milky Way::systems/Sol::linked-systems"
	$(command_arxadd) $@ --path="/Galaxies/Milky Way::systems/Alpha Centauri" --relation="/Galaxies/Milky Way::systems/Tichel::linked-systems"
	$(command_arxadd) $@ --path="/Galaxies/Milky Way::systems/Rigel" --relation="/Galaxies/Milky Way::systems/Sol::linked-systems"
	$(command_arxadd) $@ --path="/Galaxies/Milky Way::systems/Rigel" --relation="/Galaxies/Milky Way::systems/Tichel::linked-systems"
	$(command_arxadd) $@ --path="/Galaxies/Milky Way::systems/Sol" --relation="/Galaxies/Milky Way::systems/Alpha Centauri::linked-systems"
	$(command_arxadd) $@ --path="/Galaxies/Milky Way::systems/Sol" --relation="/Galaxies/Milky Way::systems/Rigel::linked-systems"
	$(command_arxadd) $@ --path="/Galaxies/Milky Way::systems/Tichel" --relation="/Galaxies/Milky Way::systems/Alpha Centauri::linked-systems"
	$(command_arxadd) $@ --path="/Galaxies/Milky Way::systems/Tichel" --relation="/Galaxies/Milky Way::systems/Rigel::linked-systems"

.data .data/galaxies .data/galaxies/milky_way:
	mkdir $@

.data/%.data: %.xml $(declarations_xml) $(xml_to_data_dependencies)
	$(command_xml_to_data) --declarations=$(declarations_xml) --in=$< --out=$@

mesh_%.xml: meshes/%.xml $(littlem_to_xml_dependencies)
	$(command_littlem_to_xml) --in=$< --out=$@

.data/fragment_shader_default.data: shaders/fragment_shader_default

.data/fragment_shader_flat.data: shaders/fragment_shader_flat

.data/fragment_shader_font.data: shaders/fragment_shader_font

.data/fragment_shader_no_lighting.data: shaders/fragment_shader_no_lighting

.data/fragment_shader_particle.data: shaders/fragment_shader_particle

.data/fragment_shader_ui_texture.data: shaders/fragment_shader_ui_texture

.data/fragment_shader_widget.data: shaders/fragment_shader_widget

.data/scenario_new_game.data: savegame_new_game.xml

.data/scenario_rich_game.data: savegame_rich_game.xml

.data/scenario_rigel_spectator.data: savegame_rigel_spectator.xml

.data/texture_particle.data: textures/generated/tex/particle.tex

.data/vertex_shader_default.data: shaders/vertex_shader_default

.data/vertex_shader_flat.data: shaders/vertex_shader_flat

.data/vertex_shader_font.data: shaders/vertex_shader_font

.data/vertex_shader_font_without_clipping.data: shaders/vertex_shader_font_without_clipping

.data/vertex_shader_no_lighting.data: shaders/vertex_shader_no_lighting

.data/vertex_shader_particle.data: shaders/vertex_shader_particle

.data/vertex_shader_ui_texture.data: shaders/vertex_shader_ui_texture

.data/vertex_shader_widget.data: shaders/vertex_shader_widget

textures-all-recursive:
	@$(MAKE) -C textures all-recursive

textures-clean-recursive:
	@$(MAKE) -C textures clean-recursive

.PHONY: all all-here all-recursive check-dependencies clean clean-here clean-recursive default meshes-all-recursive meshes-clean-recursive textures-all-recursive textures-clean-recursive

.PRECIOUS: mesh_%.xml
