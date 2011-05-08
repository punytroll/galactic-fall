root = ..

-include $(root)/configuration

HERE_SOURCE_FILES = $(wildcard *.cpp)
UI_SOURCE_FILES = $(wildcard ui/*.cpp)
SOURCE_FILES = $(HERE_SOURCE_FILES) $(UI_SOURCE_FILES)
OBJECT_FILES = $(patsubst %.cpp,.object-files/%.o,$(SOURCE_FILES))
HEADER_DEPENDENCIES = $(foreach HEADER_FILE, $(wildcard *.h), $(HEADER_FILE) '$($(patsubst %.h,%_h,$(HEADER_FILE)))')

arx_reading_h = \
	arx_reading.h

arx_resources_h = \
	arx_resources.h

asset_class_h = \
	asset_class.h

battery_h = \
	battery.h \
	$(object_h)

battery_class_h = \
	battery_class.h \
	$(type_definitions_h)

buffer_reading_h = \
	buffer_reading.h \
	$(color_h) \
	$(math_quaternion_h) \
	$(math_vector2f_h) \
	$(math_vector3f_h)

callbacks_callbacks_h = \
	callbacks/callbacks.h \
	callbacks/callbacks.inc

callbacks_connection_handle_h = \
	callbacks/connection_handle.h

callbacks_events_h = \
	callbacks/events.h \
	callbacks/events.inc \
	$(callbacks_connection_handle_h) \
	$(list_h)

camera_h = \
	camera.h \
	$(math_vector3f_h)

character_h = \
	character.h \
	$(object_h) \
	$(type_definitions_h)

character_observer_h = \
	character_observer.h

class_manager_h = \
	class_manager.h

color_h = \
	color.h \
	$(math_vector4f_h)

command_mind_h = \
	command_mind.h \
	$(mind_h)

commodity_h = \
	commodity.h \
	$(math_axis_angle_h) \
	$(math_vector3f_h) \
	$(object_h)

commodity_class_h = \
	commodity_class.h \
	$(type_definitions_h)

draw_text_h = \
	draw_text.h

faction_h = \
	faction.h \
	$(object_h)

file_handling_h = \
	file_handling.h

galaxy_h = \
	galaxy.h \
	$(object_h)

game_time_h = \
	game_time.h

generator_h = \
	generator.h \
	$(object_h)

generator_class_h = \
	generator_class.h \
	$(type_definitions_h)

globals_h = \
	globals.h

goal_h = \
	goal.h \
	$(object_h)

goals_h = \
	goals.h \
	$(goal_h) \
	$(math_vector3f_h)

graphics_engine_h = \
	graphics_engine.h \
	$(callbacks_callbacks_h)

graphics_material_h = \
	graphics_material.h

graphics_mesh_h = \
	graphics_mesh.h \
	$(math_vector4f_h)

graphics_mesh_manager_h = \
	graphics_mesh_manager.h

graphics_model_h = \
	graphics_model.h

graphics_model_manager_h = \
	graphics_model_manager.h

graphics_model_object_h = \
	graphics_model_object.h

graphics_node_h = \
	graphics_node.h \
	$(math_quaternion_h) \
	$(math_vector3f_h)

graphics_particle_system_h = \
	graphics_particle_system.h \
	$(color_h) \
	$(graphics_node_h) \
	$(math_vector2f_h)

graphics_scene_h = \
	graphics_scene.h \
	$(graphics_node_h)

graphics_texture_h = \
	graphics_texture.h \
	$(type_definitions_h)

graphics_texture_manager_h = \
	graphics_texture_manager.h

key_event_information_h = \
	key_event_information.h

list_h = \
	list.h

local_file_data_channel_h = \
	local_file_data_channel.h

main_menu_window_h = \
	main_menu_window.h \
	$(ui_dialog_h) \
	$(ui_window_h)

map_knowledge_h = \
	map_knowledge.h

math_h = \
	math.h \
	$(math_vector2f_h) \
	$(math_vector3f_h) \
	$(math_vector4d_h) \
	$(type_definitions_h)

math_axis_angle_h = \
	math/axis_angle.h \
	$(math_details_h)

math_details_h = \
	math/details.h

math_matrix3f_h = \
	math/matrix3f.h \
	$(math_details_h)

math_matrix4f_h = \
	math/matrix4f.h \
	$(math_details_h) \
	$(math_quaternion_h)

math_quaternion_h = \
	math/quaternion.h \
	$(math_details_h)

math_vector2f_h = \
	math/vector2f.h \
	$(math_details_h)

math_vector3f_h = \
	math/vector3f.h \
	$(math_details_h) \
	$(math_matrix3f_h)

math_vector4d_h = \
	math/vector4d.h

math_vector4f_h = \
	math/vector4f.h \
	$(math_details_h) \
	$(math_quaternion_h)

message_h = \
	message.h \
	$(referencing_h)

message_dispatcher_h = \
	message_dispatcher.h

messages_h = \
	messages.h

mind_h = \
	mind.h \
	$(object_h)

mini_map_display_h = \
	mini_map_display.h \
	$(referencing_h) \
	$(ui_widget_h)

object_h = \
	object.h \
	$(referencing_h)

object_aspect_accessory_h = \
	object_aspect_accessory.h

object_aspect_messages_h = \
	object_aspect_messages.h

object_aspect_name_h = \
	object_aspect_name.h

object_aspect_object_container_h = \
	object_aspect_object_container.h \
	$(callbacks_callbacks_h)

object_aspect_outfitting_h = \
	object_aspect_outfitting.h

object_aspect_physical_h = \
	object_aspect_physical.h \
	$(type_definitions_h)

object_aspect_position_h = \
	object_aspect_position.h \
	$(math_quaternion_h) \
	$(math_vector3f_h)

object_aspect_update_h = \
	object_aspect_update.h \
	$(callbacks_callbacks_h)

object_aspect_visualization_h = \
	object_aspect_visualization.h \
	$(referencing_h)

object_factory_h = \
	object_factory.h

output_observer_h = \
	output_observer.h \
	$(character_observer_h)

perspective_h = \
	perspective.h \
	$(math_vector4f_h)

planet_h = \
	planet.h \
	$(object_h) \
	$(type_definitions_h)

real_time_h = \
	real_time.h

referencing_h = \
	referencing.h

scanner_display_h = \
	scanner_display.h \
	$(referencing_h) \
	$(viewport_h)

settings_h = \
	settings.h \
	$(math_vector2f_h)

ship_h = \
	ship.h \
	$(math_vector3f_h) \
	$(object_h)

ship_class_h = \
	ship_class.h \
	$(math_vector3f_h) \
	$(type_definitions_h)

shot_h = \
	shot.h \
	$(math_vector3f_h) \
	$(object_h)

slot_h = \
	slot.h \
	$(math_quaternion_h) \
	$(math_vector3f_h) \
	$(referencing_h)

slot_class_h = \
	slot_class.h

star_h = \
	star.h \
	$(color_h) \
	$(object_h)

state_h = \
	state.h \
	$(object_h)

state_machine_h = \
	state_machine.h

states_h = \
	states.h \
	$(math_vector3f_h) \
	$(state_h)

storage_h = \
	storage.h \
	$(object_h) \
	$(type_definitions_h)

string_cast_h = \
	string_cast.h

system_h = \
	system.h \
	$(object_h)

system_statistics_h = \
	system_statistics.h \
	$(type_definitions_h)

threat_h = \
	threat.h \
	$(referencing_h)

timeout_notifications_h = \
	timeout_notifications.h \
	$(callbacks_callbacks_h)

timing_dialog_h = \
	timing_dialog.h \
	$(type_definitions_h) \
	$(ui_window_h)

type_definitions_h = \
	type_definitions.h

ui_border_h = \
	ui/border.h \
	$(color_h) \
	$(ui_widget_h)

ui_button_h = \
	ui/button.h \
	$(callbacks_events_h) \
	$(ui_widget_h)

ui_dialog_h = \
	ui/dialog.h \
	$(ui_window_h)

ui_label_h = \
	ui/label.h \
	$(color_h) \
	$(ui_widget_h)

ui_load_game_dialog_h = \
	ui/load_game_dialog.h \
	$(ui_dialog_h) \
	$(timeout_notifications_h)

ui_map_dialog_h = \
	ui/map_dialog.h \
	$(ui_window_h)

ui_object_information_dialog_h = \
	ui/object_information_dialog.h \
	$(referencing_h) \
	$(ui_window_h)

ui_outfit_ship_dialog_h = \
	ui/outfit_ship_dialog.h \
	$(ui_window_h)

ui_planet_dialog_h = \
	ui/planet_dialog.h \
	$(ui_window_h)

ui_save_game_dialog_h = \
	ui/save_game_dialog.h \
	$(ui_dialog_h) \
	$(timeout_notifications_h)

ui_scroll_bar_h = \
	ui/scroll_bar.h \
	$(ui_widget_h)

ui_scroll_box_h = \
	ui/scroll_box.h \
	$(ui_widget_h)

ui_star_map_display_h = \
	ui/star_map_display.h \
	$(ui_widget_h)

ui_trade_center_dialog_h = \
	ui/trade_center_dialog.h \
	$(ui_window_h)

ui_widget_h = \
	ui/widget.h \
	$(callbacks_events_h) \
	$(math_vector2f_h)

ui_window_h = \
	ui/window.h \
	$(ui_widget_h)

user_interface_h = \
	user_interface.h

viewport_h = \
	viewport.h \
	$(camera_h) \
	$(perspective_h) \
	$(ui_widget_h)

visualization_prototype_h = \
	visualization_prototype.h

visualizations_h = \
	visualizations.h

weapon_h = \
	weapon.h \
	$(object_h)

weapon_class_h = \
	weapon_class.h \
	$(math_quaternion_h) \
	$(math_vector3f_h) \
	$(type_definitions_h)

write_to_xml_stream_h = \
	write_to_xml_stream.h

xml_parser_h = \
	xml_parser.h

xml_puny_dom_h = \
	xml_puny_dom.h

xml_stream_h = \
	xml_stream.h

default: all

all: all-recursive

all-here: check-dependencies check-consistency prepare-build binary

all-recursive: callbacks-all-recursive all-here

clean: clean-recursive

clean-here:
	$(RM) binary
	$(RM) $(OBJECT_FILES)
	@MORE_OBJECT_FILES="$(filter-out $(OBJECT_FILES),$(wildcard .object-files/*.o))"; \
	if [ x"$${MORE_OBJECT_FILES}" != x ]; then \
		echo "\nThe following object files have not been removed explicitly. They are deleted with the \".object-files\" directory, nevertheless."; \
		for OBJECT_FILE in $${MORE_OBJECT_FILES}; do \
			echo "    $${OBJECT_FILE}"; \
		done; \
		echo ""; \
	fi
	$(RM) -r .object-files

clean-recursive: callbacks-clean-recursive clean-here

callbacks-all-recursive:
	@$(MAKE) -C callbacks all-recursive

callbacks-clean-recursive:
	@$(MAKE) -C callbacks clean-recursive

check-dependencies:
	@if [ x`which pkg-config` = x ]; then \
		echo -e "\nYour system is missing the \"pkg-config\" tool.\nThis tool is needed and you will need to install it in order to build the program.\n"; false; \
	fi
	@if ! pkg-config --exists libarxx; then \
		echo -e "\nYour system is missing the \"libarxx\" library.\nYou can get this library from \"http://libarxx.sourceforge.net\"./\n"; false; \
	fi
	@if ! pkg-config --atleast-version=0.7.9 libarxx; then \
		echo -e "\nThe library \"libarxx\" that is installed on your system is too old.\nYou have: Version `pkg-config --modversion libarxx`\nYou need: Version 0.7.9\nYou can get the most recent library from \"http://libarxx.sourceforge.net\"./\n"; false; \
	fi

check-consistency:
	@HEADER_FILE=; \
	ERROR="no"; \
	for HEADER_DEPENDENCY in $(HEADER_DEPENDENCIES); do \
		if [ -z $${HEADER_FILE} ]; then \
			HEADER_FILE=$${HEADER_DEPENDENCY}; \
		else \
			if [ -z "$${HEADER_DEPENDENCY}" ]; then \
				echo -e "\nThe build system contains no dependency specification for the header file '"$${HEADER_FILE}"'."; \
				ERROR="yes"; \
			else \
				for HEADER_FILE_DEPENDENCY in $${HEADER_DEPENDENCY}; do \
					if [ ! -f $${HEADER_FILE_DEPENDENCY} ]; then \
						echo -e "\nThe build system contains a reference to the non-existent file '"$${HEADER_FILE_DEPENDENCY}"' for the header file '"$${HEADER_FILE}"'."; \
						ERROR="yes"; \
					fi; \
				done; \
			fi; \
			HEADER_FILE=; \
		fi; \
	done; \
	if [ $${ERROR} = "yes" ]; then \
		echo ""; \
		exit 1; \
	fi

.object-files:
	mkdir $@

.object-files/ui:
	mkdir $@

prepare-build: .object-files .object-files/ui

$(OBJECT_FILES): $(CXXFLAGS_configuration)

binary: $(OBJECT_FILES) $(LDFLAGS_configuration)
	$(CXX) $(LDFLAGS) `pkg-config --libs libarxx` $(OBJECT_FILES) -lGL -lX11 -o $@

.object-files/arx_reading.o: arx_reading.cpp $(arx_reading_h) $(arx_resources_h) $(asset_class_h) $(battery_class_h) $(buffer_reading_h) $(callbacks_callbacks_h) $(class_manager_h) $(commodity_class_h) $(faction_h) $(galaxy_h) $(generator_class_h) $(globals_h) $(graphics_material_h) $(graphics_mesh_h) $(graphics_mesh_manager_h) $(graphics_model_h) $(graphics_model_manager_h) $(graphics_texture_h) $(graphics_texture_manager_h) $(local_file_data_channel_h) $(mini_map_display_h) $(object_aspect_name_h) $(object_aspect_object_container_h) $(object_aspect_position_h) $(object_factory_h) $(planet_h) $(scanner_display_h) $(settings_h) $(ship_class_h) $(slot_h) $(star_h) $(string_cast_h) $(system_h) $(ui_label_h) $(ui_widget_h) $(user_interface_h) $(visualization_prototype_h) $(weapon_class_h)
	$(CXX) $(CXXFLAGS) `pkg-config --cflags libarxx` -c $< -o $@

.object-files/asset_class.o: asset_class.cpp $(asset_class_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

.object-files/battery.o: battery.cpp $(battery_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

.object-files/battery_class.o: battery_class.cpp $(battery_class_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

.object-files/buffer_reading.o: buffer_reading.cpp $(buffer_reading_h)
	$(CXX) $(CXXFLAGS) `pkg-config --cflags libarxx` -c $< -o $@

.object-files/camera.o: camera.cpp $(camera_h) $(object_h) $(object_aspect_position_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

.object-files/character.o: character.cpp $(character_h) $(character_observer_h) $(map_knowledge_h) $(message_h) $(mind_h) $(object_aspect_messages_h) $(object_aspect_object_container_h) $(threat_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

.object-files/character_observer.o: character_observer.cpp $(character_h) $(character_observer_h) $(message_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

.object-files/color.o: color.cpp $(color_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

.object-files/command_mind.o: command_mind.cpp $(commodity_h) $(character_h) $(command_mind_h) $(object_aspect_position_h) $(planet_h) $(ship_h) $(system_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

.object-files/commodity.o: commodity.cpp $(commodity_h) $(math_vector4f_h) $(object_aspect_position_h) $(object_aspect_update_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

.object-files/commodity_class.o: commodity_class.cpp $(color_h) $(commodity_class_h) $(visualization_prototype_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

.object-files/draw_text.o: draw_text.cpp $(draw_text_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

.object-files/faction.o: faction.cpp $(color_h) $(faction_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

.object-files/file_handling.o: file_handling.cpp $(file_handling_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

.object-files/galaxy.o: galaxy.cpp $(faction_h) $(galaxy_h) $(object_aspect_object_container_h) $(system_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

.object-files/game_time.o: game_time.cpp $(game_time_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

.object-files/generator.o: generator.cpp $(generator_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

.object-files/generator_class.o: generator_class.cpp $(generator_class_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

.object-files/goal.o: goal.cpp $(goal_h) $(object_aspect_object_container_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

.object-files/goals.o: goals.cpp $(asset_class_h) $(character_h) $(commodity_h) $(faction_h) $(game_time_h) $(globals_h) $(goals_h) $(math_h) $(message_h) $(messages_h) $(mind_h) $(object_aspect_position_h) $(planet_h) $(ship_h) $(string_cast_h) $(system_h) $(threat_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

.object-files/graphics_engine.o: graphics_engine.cpp $(graphics_engine_h) $(graphics_scene_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

.object-files/graphics_material.o: graphics_material.cpp $(color_h) $(graphics_material_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

.object-files/graphics_mesh.o: graphics_mesh.cpp $(graphics_mesh_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

.object-files/graphics_mesh_manager.o: graphics_mesh_manager.cpp $(graphics_mesh_h) $(graphics_mesh_manager_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

.object-files/graphics_model.o: graphics_model.cpp $(graphics_mesh_h) $(graphics_model_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

.object-files/graphics_model_manager.o: graphics_model_manager.cpp $(graphics_model_h) $(graphics_model_manager_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

.object-files/graphics_model_object.o: graphics_model_object.cpp $(color_h) $(graphics_mesh_h) $(graphics_model_h) $(graphics_model_object_h) $(math_matrix4f_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

.object-files/graphics_node.o: graphics_node.cpp $(graphics_engine_h) $(graphics_node_h) $(math_matrix4f_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

.object-files/graphics_particle_system.o: graphics_particle_system.cpp $(color_h) $(game_time_h) $(globals_h) $(graphics_particle_system_h) $(graphics_texture_h) $(graphics_texture_manager_h) $(math_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

.object-files/graphics_scene.o: graphics_scene.cpp $(graphics_particle_system_h) $(graphics_scene_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

.object-files/graphics_texture.o: graphics_texture.cpp $(graphics_texture_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

.object-files/graphics_texture_manager.o: graphics_texture_manager.cpp $(graphics_texture_h) $(graphics_texture_manager_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

.object-files/key_event_information.o: key_event_information.cpp $(key_event_information_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

.object-files/local_file_data_channel.o: local_file_data_channel.cpp $(local_file_data_channel_h)
	$(CXX) $(CXXFLAGS) `pkg-config --cflags libarxx` -c $< -o $@

.object-files/main.o: main.cpp $(arx_reading_h) $(asset_class_h) $(battery_h) $(battery_class_h) $(camera_h) $(commodity_h) $(commodity_class_h) $(character_h) $(color_h) $(command_mind_h) $(draw_text_h) $(faction_h) $(file_handling_h) $(galaxy_h) $(game_time_h) $(generator_h) $(generator_class_h) $(goals_h) $(graphics_engine_h) $(graphics_mesh_manager_h) $(graphics_model_h) $(graphics_model_manager_h) $(graphics_model_object_h) $(graphics_particle_system_h) $(graphics_scene_h) $(graphics_texture_manager_h) $(key_event_information_h) $(main_menu_window_h) $(map_knowledge_h) $(math_h) $(message_h) $(message_dispatcher_h) $(messages_h) $(mind_h) $(mini_map_display_h) $(object_aspect_accessory_h) $(object_aspect_name_h) $(object_aspect_object_container_h) $(object_aspect_outfitting_h) $(object_aspect_physical_h) $(object_aspect_position_h) $(object_aspect_update_h) $(object_aspect_visualization_h) $(object_factory_h) $(output_observer_h) $(perspective_h) $(planet_h) $(real_time_h) $(scanner_display_h) $(settings_h) $(ship_h) $(ship_class_h) $(shot_h) $(slot_h) $(slot_class_h) $(star_h) $(state_machine_h) $(states_h) $(storage_h) $(string_cast_h) $(system_h) $(system_statistics_h) $(timeout_notifications_h) $(timing_dialog_h) $(ui_button_h) $(ui_label_h) $(ui_map_dialog_h) $(ui_object_information_dialog_h) $(ui_outfit_ship_dialog_h) $(ui_star_map_display_h) $(ui_widget_h) $(user_interface_h) $(visualizations_h) $(weapon_h) $(weapon_class_h) $(write_to_xml_stream_h) $(xml_puny_dom_h) $(xml_stream_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

.object-files/main_menu_window.o: main_menu_window.cpp $(callbacks_callbacks_h) $(file_handling_h) $(key_event_information_h) $(main_menu_window_h) $(ui_button_h) $(ui_label_h) $(ui_load_game_dialog_h) $(ui_save_game_dialog_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

.object-files/map_knowledge.o: map_knowledge.cpp $(map_knowledge_h) $(system.h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

.object-files/message.o: message.cpp $(message_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

.object-files/message_dispatcher.o: message_dispatcher.cpp $(globals_h) $(message_h) $(message_dispatcher_h) $(object_h) $(object_aspect_messages_h) $(referencing_h) $(system_statistics_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

.object-files/messages.o: messages.cpp $(messages_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

.object-files/mind.o: mind.cpp $(goal_h) $(mind_h) $(object_aspect_object_container_h) $(state_machine_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

.object-files/mini_map_display.o: mini_map_display.cpp $(commodity_h) $(mini_map_display_h) $(object_aspect_position_h) $(planet_h) $(ship_h) $(system_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

.object-files/object.o: object.cpp $(message_h) $(object_h) $(object_aspect_accessory_h) $(object_aspect_messages_h) $(object_aspect_name_h) $(object_aspect_object_container_h) $(object_aspect_outfitting_h) $(object_aspect_physical_h) $(object_aspect_position_h) $(object_aspect_update_h) $(object_aspect_visualization_h) $(real_time_h) $(string_cast_h) $(xml_stream_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

.object-files/object_aspect_accessory.o: object_aspect_accessory.cpp $(object_aspect_accessory_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

.object-files/object_aspect_messages.o: object_aspect_messages.cpp $(object_aspect_messages_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

.object-files/object_aspect_object_container.o: object_aspect_object_container.cpp $(object_h) $(object_aspect_object_container_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

.object-files/object_aspect_outfitting.o: object_aspect_outfitting.cpp $(object_aspect_outfitting_h) $(slot_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

.object-files/object_aspect_physical.o: object_aspect_physical.cpp $(object_aspect_physical_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

.object-files/object_aspect_position.o: object_aspect_position.cpp $(object_aspect_position_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

.object-files/object_aspect_update.o: object_aspect_update.cpp $(object_aspect_update_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

.object-files/object_aspect_visualization.o: object_aspect_visualization.cpp $(graphics_node_h) $(object_aspect_visualization_h) $(visualization_prototype_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

.object-files/object_factory.o: object_factory.cpp $(battery_h) $(battery_class_h) $(character_h) $(class_manager_h) $(command_mind_h) $(commodity_h) $(commodity_class_h) $(faction_h) $(game_time_h) $(generator_h) $(generator_class_h) $(globals_h) $(graphics_model_h) $(object_aspect_name_h) $(object_aspect_outfitting_h) $(object_aspect_physical_h) $(object_aspect_position_h) $(object_aspect_visualization_h) $(object_factory_h) $(planet_h) $(ship_h) $(ship_class_h) $(shot_h) $(slot_h) $(storage_h) $(weapon_h) $(weapon_class_h) $(visualization_prototype_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

.object-files/output_observer.o: output_observer.cpp $(command_mind_h) $(globals_h) $(message_h) $(output_observer_h) $(planet_h) $(ui_planet_dialog_h) $(user_interface_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

.object-files/perspective.o: perspective.cpp $(perspective_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

.object-files/planet.o: planet.cpp $(asset_class_h) $(battery_h) $(color_h) $(globals_h) $(message_h) $(message_dispatcher_h) $(object_aspect_object_container_h) $(object_aspect_physical_h) $(planet_h) $(ship_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

.object-files/real_time.o: real_time.cpp $(real_time_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

.object-files/scanner_display.o: scanner_display.cpp $(object_aspect_physical_h) $(scanner_display_h) $(ship_h) $(star_h) $(system.h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

.object-files/settings.o: settings.cpp $(arx_resources_h) $(buffer_reading_h) $(settings_h) $(string_cast_h)
	$(CXX) $(CXXFLAGS) `pkg-config --cflags libarxx` -c $< -o $@

.object-files/ship.o: ship.cpp $(battery_h) $(character_h) $(commodity_h) $(game_time_h) $(generator_h) $(globals_h) $(graphics_particle_system_h) $(map_knowledge_h) $(math_h) $(message_h) $(message_dispatcher_h) $(object_aspect_accessory_h) $(object_aspect_object_container_h) $(object_aspect_outfitting_h) $(object_aspect_position_h) $(object_aspect_update_h) $(object_aspect_visualization_h) $(planet_h) $(ship_h) $(slot_h) $(slot_class_h) $(storage_h) $(system_h) $(visualizations_h) $(weapon_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

.object-files/ship_class.o: ship_class.cpp $(color_h) $(graphics_material_h) $(ship_class_h) $(slot_h) $(visualization_prototype_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

.object-files/shot.o: shot.cpp $(game_time_h) $(object_aspect_position_h) $(object_aspect_update_h) $(shot_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

.object-files/slot.o: slot.cpp $(object_h) $(object_aspect_accessory_h) $(object_aspect_visualization_h) $(slot_h) $(slot_class_h) $(visualizations_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

.object-files/slot_class.o: slot_class.cpp $(slot_class_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

.object-files/star.o: star.cpp $(star_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

.object-files/state.o: state.cpp $(state_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

.object-files/state_machine.o: state_machine.cpp $(state_h) $(state_machine_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

.object-files/states.o: states.cpp $(asset_class_h) $(character_h) $(commodity_h) $(commodity_class_h) $(game_time_h) $(globals_h) $(math_h) $(message_h) $(mind_h) $(object_aspect_object_container_h) $(object_aspect_physical_h) $(object_aspect_position_h) $(object_factory_h) $(planet_h) $(ship_h) $(state_machine_h) $(states_h) $(storage_h) $(string_cast_h) $(system_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

.object-files/storage.o: storage.cpp $(object_aspect_object_container_h) $(object_aspect_physical_h) $(storage_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

.object-files/string_cast.o: string_cast.cpp $(string_cast_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

.object-files/system.o: system.cpp $(commodity_h) $(object_aspect_object_container_h) $(planet_h) $(ship_h) $(shot_h) $(star_h) $(system_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

.object-files/threat.o: threat.cpp $(threat_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

.object-files/timeout_notifications.o: timeout_notifications.cpp $(timeout_notifications_h) $(type_definitions_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

.object-files/timing_dialog.o: timing_dialog.cpp $(callbacks_callbacks_h) $(globals_h) $(key_event_information_h) $(string_cast_h) $(system_statistics_h) $(timing_dialog_h) $(ui_label_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

.object-files/ui/border.o: ui/border.cpp $(ui_border_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

.object-files/ui/button.o: ui/button.cpp $(callbacks_callbacks_h) $(color_h) $(globals_h) $(ui_button_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

.object-files/ui/dialog.o: ui/dialog.cpp $(callbacks_callbacks_h) $(ui_dialog_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

.object-files/ui/label.o: ui/label.cpp $(ui_label_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

.object-files/ui/load_game_dialog.o: ui/load_game_dialog.cpp $(globals_h) $(key_event_information_h) $(real_time_h) $(ui_button_h) $(ui_label_h) $(ui_load_game_dialog_h) $(ui_scroll_box_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

.object-files/ui/map_dialog.o: ui/map_dialog.cpp $(callbacks_callbacks_h) $(globals_h) $(key_event_information_h) $(object_aspect_name_h) $(system_h) $(ui_button_h) $(ui_label_h) $(ui_map_dialog_h) $(ui_star_map_display_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

.object-files/ui/object_information_dialog.o: ui/object_information_dialog.cpp  $(globals_h) $(object_h) $(object_aspect_accessory_h) $(object_aspect_name_h) $(object_aspect_object_container_h) $(object_aspect_physical_h)  $(slot_h) $(slot_class_h) $(string_cast_h) $(ui_button_h) $(ui_label_h) $(ui_object_information_dialog_h) $(ui_scroll_bar_h) $(ui_scroll_box_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

.object-files/ui/outfit_ship_dialog.o: ui/outfit_ship_dialog.cpp $(class_manager_h) $(color_h) $(globals_h) $(key_event_information_h) $(object_aspect_accessory_h) $(object_aspect_name_h) $(object_aspect_object_container_h) $(object_aspect_outfitting_h) $(object_aspect_physical_h) $(ship_h) $(slot_h) $(slot_class_h) $(storage_h) $(ui_button_h) $(ui_label_h) $(ui_outfit_ship_dialog_h) $(ui_scroll_box_h) $(weapon_h) $(weapon_class_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

.object-files/ui/planet_dialog.o: ui/planet_dialog.cpp $(asset_class_h) $(callbacks_callbacks_h) $(character_h) $(color_h) $(globals_h) $(key_event_information_h) $(object_aspect_name_h) $(planet_h) $(ship_h) $(string_cast_h) $(ui_button_h) $(ui_label_h) $(ui_planet_dialog_h) $(ui_trade_center_dialog_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

.object-files/ui/save_game_dialog.o: ui/save_game_dialog.cpp $(globals_h) $(key_event_information_h) $(real_time_h) $(ui_button_h) $(ui_label_h) $(ui_save_game_dialog_h) $(ui_scroll_box_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

.object-files/ui/scroll_bar.o: ui/scroll_bar.cpp $(callbacks_callbacks_h) $(color_h) $(globals_h) $(ui_button_h) $(ui_scroll_bar_h) $(user_interface_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

.object-files/ui/scroll_box.o: ui/scroll_box.cpp $(callbacks_callbacks_h) $(color_h) $(ui_scroll_bar_h) $(ui_scroll_box_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

.object-files/ui/star_map_display.o: ui/star_map_display.cpp $(callbacks_callbacks_h) $(character_h) $(draw_text_h) $(galaxy_h) $(globals_h) $(map_knowledge_h) $(object_aspect_name_h) $(object_aspect_position_h) $(system_h) $(ui_star_map_display_h) $(user_interface_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

.object-files/ui/trade_center_dialog.o: ui/trade_center_dialog.cpp $(asset_class_h) $(character_h) $(commodity_h) $(commodity_class_h) $(character_h) $(game_time_h) $(graphics_model_manager_h) $(key_event_information_h) $(object_aspect_accessory_h) $(object_aspect_name_h) $(object_aspect_object_container_h) $(object_factory_h) $(planet_h) $(ship_h) $(storage_h) $(ui_button_h) $(ui_label_h) $(ui_scroll_bar_h) $(ui_scroll_box_h) $(ui_trade_center_dialog_h) $(weapon_h) $(weapon_class_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

.object-files/ui/widget.o: ui/widget.cpp $(callbacks_callbacks_h) $(color_h) $(math_h) $(ui_widget_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

.object-files/ui/window.o: ui/window.cpp $(callbacks_callbacks_h) $(globals_h) $(user_interface_h) $(ui_border_h) $(ui_label_h) $(ui_window_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

.object-files/user_interface.o: user_interface.cpp $(callbacks_connection_handle_h) $(ui_widget_h) $(user_interface_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

.object-files/viewport.o: viewport.cpp $(viewport_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

.object-files/visualization_prototype.o: visualization_prototype.cpp $(graphics_material_h) $(visualization_prototype_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

.object-files/visualizations.o: visualizations.cpp $(color_h) $(commodity_h) $(commodity_class_h) $(globals_h) $(graphics_material_h) $(graphics_model_h) $(graphics_model_object_h) $(graphics_model_manager_h) $(graphics_particle_system_h) $(object_aspect_accessory_h) $(object_aspect_outfitting_h) $(object_aspect_physical_h) $(object_aspect_position_h) $(object_aspect_visualization_h) $(planet_h) $(ship_h) $(shot_h) $(visualization_prototype_h) $(visualizations_h) $(weapon_h) $(weapon_class_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

.object-files/weapon.o: weapon.cpp $(battery_h) $(game_time_h) $(globals_h) $(math_h) $(math_quaternion_h) $(object_aspect_accessory_h) $(object_aspect_object_container_h) $(object_aspect_position_h) $(object_factory_h) $(ship_h) $(shot_h) $(slot_h) $(string_cast_h) $(visualizations_h) $(weapon_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

.object-files/weapon_class.o: weapon_class.cpp $(color_h) $(visualization_prototype_h) $(weapon_class_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

.object-files/write_to_xml_stream.o: write_to_xml_stream.cpp $(battery_h) $(character_h) $(color_h) $(commodity_h) $(faction_h) $(generator_h) $(graphics_material_h) $(map_knowledge_h) $(mind_h) $(object_h) $(object_aspect_accessory_h) $(object_aspect_name_h) $(object_aspect_object_container_h) $(object_aspect_physical_h) $(object_aspect_position_h) $(object_aspect_visualization_h) $(ship_h) $(slot_h) $(storage_h) $(system_h) $(visualization_prototype_h) $(weapon_h) $(write_to_xml_stream_h) $(xml_stream_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

.object-files/xml_parser.o: xml_parser.cpp $(xml_parser_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

.object-files/xml_puny_dom.o: xml_puny_dom.cpp $(xml_parser_h) $(xml_puny_dom_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

.object-files/xml_stream.o: xml_stream.cpp $(xml_stream_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

