root = ..

default: all

include $(root)/tools/littlem_to_xml/Makefile.include
include $(root)/tools/xml_to_data/Makefile.include

all_data = \
	.data \
	.data/ammunition_class_light_gauss_ammunition.data \
	.data/battery_class_light_battery.data \
	.data/commodity_class_equipment.data \
	.data/commodity_class_food.data \
	.data/commodity_class_fuel.data \
	.data/commodity_class_industrial.data \
	.data/commodity_class_luxury_goods.data \
	.data/commodity_class_medical_supplies.data \
	.data/commodity_class_metal.data \
	.data/energy_projectile_weapon_class_light_gauss_cannon.data \
	.data/energy_projectile_weapon_class_light_laser.data \
	.data/energy_projectile_weapon_class_medium_laser.data \
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
	.data/mesh_gauss_shell.data \
	.data/mesh_laser_shot.data \
	.data/mesh_light_battery.data \
	.data/mesh_light_gauss_ammunition.data \
	.data/mesh_light_gauss_cannon.data \
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
	.data/model_gauss_shell.data \
	.data/model_laser_shot.data \
	.data/model_light_battery.data \
	.data/model_light_gauss_ammunition.data \
	.data/model_light_gauss_cannon.data \
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
	.data/vertex_shader_widget.data

declarations_xml = \
	tools/declarations.xml

littlem_to_xml_xsl = \
	tools/littlem_to_xml.xsl

all: all-recursive

all-here: $(all_data)

all-recursive: textures-all-recursive all-here

clean: clean-recursive

clean-here:
	$(RM) -r .data
	$(RM) mesh_cargo_cube.xml
	$(RM) mesh_fighter_cockpit.xml
	$(RM) mesh_fighter_faction.xml
	$(RM) mesh_fighter_hull.xml
	$(RM) mesh_gauss_shell.xml
	$(RM) mesh_laser_shot.xml
	$(RM) mesh_light_battery.xml
	$(RM) mesh_light_gauss_ammunition.xml
	$(RM) mesh_light_gauss_cannon.xml
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

.PHONY: all all-here all-recursive clean clean-here clean-recursive default meshes-all-recursive meshes-clean-recursive textures-all-recursive textures-clean-recursive

.PRECIOUS: mesh_%.xml
