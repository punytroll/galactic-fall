root = ..

-include $(root)/configuration

SOURCE_FILES = $(wildcard *.cpp)
OBJECT_FILES = $(patsubst %.cpp,%.o,$(SOURCE_FILES))
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

border_h = \
	border.h \
	$(color_h) \
	$(widget_h)

buffer_reading_h = \
	buffer_reading.h \
	$(color_h) \
	$(math_quaternion_h) \
	$(math_vector2f_h) \
	$(math_vector3f_h)

button_h = \
	button.h \
	$(mouse_button_listener_h) \
	$(mouse_motion_listener_h) \
	$(widget_h)

callbacks_callbacks_h = \
	callbacks/callbacks.h \
	callbacks/callbacks.inc

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

clicked_listener_h = \
	clicked_listener.h

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

destroy_listener_h = \
	destroy_listener.h

dimension_listener_h = \
	dimension_listener.h

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

key_listener_h = \
	key_listener.h

label_h = \
	label.h \
	$(color_h) \
	$(widget_h)

local_file_data_channel_h = \
	local_file_data_channel.h

load_game_dialog_h = \
	load_game_dialog.h \
	$(callbacks_callbacks_h) \
	$(clicked_listener_h) \
	$(key_listener_h) \
	$(mouse_button_listener_h) \
	$(timeout_notifications_h) \
	$(window_h)

map_dialog_h = \
	map_dialog.h \
	$(clicked_listener_h) \
	$(key_listener_h) \
	$(window_h)

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
	$(widget_h)

mouse_button_listener_h = \
	mouse_button_listener.h

mouse_motion_listener_h = \
	mouse_motion_listener.h

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

object_information_dialog_h = \
	object_information_dialog.h \
	$(clicked_listener_h) \
	$(referencing_h) \
	$(window_h)

outfit_ship_dialog_h = \
	outfit_ship_dialog.h \
	$(clicked_listener_h) \
	$(dimension_listener_h) \
	$(key_listener_h) \
	$(mouse_button_listener_h) \
	$(window_h)

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

planet_dialog_h = \
	planet_dialog.h \
	$(clicked_listener_h) \
	$(destroy_listener_h) \
	$(key_listener_h) \
	$(window_h)

real_time_h = \
	real_time.h

referencing_h = \
	referencing.h

save_game_dialog_h = \
	save_game_dialog.h \
	$(callbacks_callbacks_h) \
	$(clicked_listener_h) \
	$(key_listener_h) \
	$(mouse_button_listener_h) \
	$(timeout_notifications_h) \
	$(window_h)

scanner_display_h = \
	scanner_display.h \
	$(referencing_h) \
	$(viewport_h)

scroll_bar_h = \
	scroll_bar.h \
	$(clicked_listener_h) \
	$(dimension_listener_h) \
	$(mouse_button_listener_h) \
	$(mouse_motion_listener_h) \
	$(widget_h)

scroll_box_h = \
	scroll_box.h \
	$(dimension_listener_h) \
	$(scroll_position_changed_listener_h) \
	$(widget_h)

scroll_position_changed_listener_h = \
	scroll_position_changed_listener.h \

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

star_map_display_h = \
	star_map_display.h \
	$(mouse_button_listener_h) \
	$(mouse_motion_listener_h) \
	$(widget_h)

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
	$(window_h)

trade_center_dialog_h = \
	trade_center_dialog.h \
	$(clicked_listener_h) \
	$(key_listener_h) \
	$(mouse_button_listener_h) \
	$(mouse_motion_listener_h) \
	$(window_h)

type_definitions_h = \
	type_definitions.h

user_interface_h = \
	user_interface.h

viewport_h = \
	viewport.h \
	$(camera_h) \
	$(perspective_h) \
	$(widget_h)

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

widget_h = \
	widget.h \
	$(math_vector2f_h)

window_h = \
	window.h \
	$(mouse_button_listener_h) \
	$(mouse_motion_listener_h) \
	$(widget_h)

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

all-here: check-dependencies check-consistency binary

all-recursive: callbacks-all-recursive all-here

clean: clean-recursive

clean-here:
	$(RM) binary
	$(RM) $(OBJECT_FILES)
	@MORE_OBJECT_FILES="$(filter-out $(OBJECT_FILES),$(wildcard *.o))"; \
	if [ x"$${MORE_OBJECT_FILES}" != x ]; then \
		echo -e "\nThe following object files have not been removed automatically. You may want to remove them by hand."; \
		for OBJECT_FILE in $${MORE_OBJECT_FILES}; do \
			echo -e "\t$${OBJECT_FILE}"; \
		done; \
		echo ""; \
	fi

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
	@if ! pkg-config --atleast-version=0.7.8 libarxx; then \
		echo -e "\nThe library \"libarxx\" that is installed on your system is too old.\nYou have: Version `pkg-config --modversion libarxx`\nYou need: Version 0.7.8\nYou can get the most recent library from \"http://libarxx.sourceforge.net\"./\n"; false; \
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

$(OBJECT_FILES): $(CXXFLAGS_configuration)

binary: $(OBJECT_FILES) $(LDFLAGS_configuration)
	$(CXX) $(LDFLAGS) `pkg-config --libs libarxx` $(OBJECT_FILES) -lGL -o $@

arx_reading.o: arx_reading.cpp $(arx_reading_h) $(arx_resources_h) $(asset_class_h) $(battery_class_h) $(buffer_reading_h) $(class_manager_h) $(commodity_class_h) $(faction_h) $(galaxy_h) $(generator_class_h) $(globals_h) $(graphics_material_h) $(graphics_mesh_h) $(graphics_mesh_manager_h) $(graphics_model_h) $(graphics_model_manager_h) $(graphics_texture_h) $(graphics_texture_manager_h) $(label_h) $(local_file_data_channel_h) $(mini_map_display_h) $(object_aspect_name_h) $(object_aspect_object_container_h) $(object_aspect_position_h) $(object_factory_h) $(planet_h) $(scanner_display_h) $(ship_class_h) $(callbacks_callbacks_h) $(slot_h) $(star_h) $(string_cast_h) $(system_h) $(user_interface_h) $(visualization_prototype_h) $(weapon_class_h) $(widget_h)
	$(CXX) $(CXXFLAGS) `pkg-config --cflags libarxx` -c $< -o $@

asset_class.o: asset_class.cpp $(asset_class_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

battery.o: battery.cpp $(battery_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

battery_class.o: battery_class.cpp $(battery_class_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

border.o: border.cpp $(border_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

buffer_reading.o: buffer_reading.cpp $(buffer_reading_h)
	$(CXX) $(CXXFLAGS) `pkg-config --cflags libarxx` -c $< -o $@

button.o: button.cpp $(button_h) $(clicked_listener_h) $(color_h) $(globals_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

camera.o: camera.cpp $(camera_h) $(object_h) $(object_aspect_position_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

character.o: character.cpp $(character_h) $(character_observer_h) $(map_knowledge_h) $(message_h) $(mind_h) $(object_aspect_messages_h) $(object_aspect_object_container_h) $(threat_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

character_observer.o: character_observer.cpp $(character_h) $(character_observer_h) $(message_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clicked_listener.o: clicked_listener.cpp $(clicked_listener_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

color.o: color.cpp $(color_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

command_mind.o: command_mind.cpp $(commodity_h) $(character_h) $(command_mind_h) $(object_aspect_position_h) $(planet_h) $(ship_h) $(system_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

commodity.o: commodity.cpp $(commodity_h) $(math_vector4f_h) $(object_aspect_position_h) $(object_aspect_update_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

commodity_class.o: commodity_class.cpp $(color_h) $(commodity_class_h) $(visualization_prototype_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

destroy_listener.o: destroy_listener.cpp $(destroy_listener_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

dimension_listener.o: dimension_listener.cpp $(dimension_listener_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

draw_text.o: draw_text.cpp $(draw_text_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

faction.o: faction.cpp $(color_h) $(faction_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

file_handling.o: file_handling.cpp $(file_handling_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

galaxy.o: galaxy.cpp $(faction_h) $(galaxy_h) $(object_aspect_object_container_h) $(system_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

game_time.o: game_time.cpp $(game_time_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

generator.o: generator.cpp $(generator_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

generator_class.o: generator_class.cpp $(generator_class_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

goal.o: goal.cpp $(goal_h) $(object_aspect_object_container_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

goals.o: goals.cpp $(asset_class_h) $(character_h) $(commodity_h) $(game_time_h) $(globals_h) $(goals_h) $(math_h) $(message_h) $(messages_h) $(mind_h) $(object_aspect_position_h) $(planet_h) $(ship_h) $(string_cast_h) $(system_h) $(threat_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

graphics_engine.o: graphics_engine.cpp $(graphics_engine_h) $(graphics_scene_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

graphics_material.o: graphics_material.cpp $(color_h) $(graphics_material_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

graphics_mesh.o: graphics_mesh.cpp $(graphics_mesh_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

graphics_mesh_manager.o: graphics_mesh_manager.cpp $(graphics_mesh_h) $(graphics_mesh_manager_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

graphics_model.o: graphics_model.cpp $(graphics_mesh_h) $(graphics_model_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

graphics_model_manager.o: graphics_model_manager.cpp $(graphics_model_h) $(graphics_model_manager_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

graphics_model_object.o: graphics_model_object.cpp $(color_h) $(graphics_mesh_h) $(graphics_model_h) $(graphics_model_object_h) $(math_matrix4f_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

graphics_node.o: graphics_node.cpp $(graphics_engine_h) $(graphics_node_h) $(math_matrix4f_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

graphics_particle_system.o: graphics_particle_system.cpp $(color_h) $(game_time_h) $(globals_h) $(graphics_particle_system_h) $(graphics_texture_h) $(graphics_texture_manager_h) $(math_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

graphics_scene.o: graphics_scene.cpp $(graphics_particle_system_h) $(graphics_scene_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

graphics_texture.o: graphics_texture.cpp $(graphics_texture_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

graphics_texture_manager.o: graphics_texture_manager.cpp $(graphics_texture_h) $(graphics_texture_manager_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

key_event_information.o: key_event_information.cpp $(key_event_information_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

key_listener.o: key_listener.cpp $(key_listener_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

label.o: label.cpp $(label_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

load_game_dialog.o: load_game_dialog.cpp $(button_h) $(globals_h) $(key_event_information_h) $(label_h) $(load_game_dialog_h) $(real_time_h) $(scroll_box_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

local_file_data_channel.o: local_file_data_channel.cpp $(local_file_data_channel_h)
	$(CXX) $(CXXFLAGS) `pkg-config --cflags libarxx` -c $< -o $@

main.o: main.cpp $(arx_reading_h) $(asset_class_h) $(battery_h) $(battery_class_h) $(camera_h) $(commodity_h) $(commodity_class_h) $(character_h) $(color_h) $(command_mind_h) $(destroy_listener_h) $(galaxy_h) $(game_time_h) $(generator_h) $(generator_class_h) $(goals_h) $(graphics_engine_h) $(graphics_mesh_manager_h) $(graphics_model_h) $(graphics_model_manager_h) $(graphics_model_object_h) $(graphics_particle_system_h) $(graphics_scene_h) $(graphics_texture_manager_h) $(key_event_information_h) $(label_h) $(load_game_dialog_h) $(map_dialog_h) $(map_knowledge_h) $(math_h) $(message_h) $(message_dispatcher_h) $(messages_h) $(mind_h) $(mini_map_display_h) $(object_aspect_accessory_h) $(object_aspect_name_h) $(object_aspect_object_container_h) $(object_aspect_outfitting_h) $(object_aspect_physical_h) $(object_aspect_position_h) $(object_aspect_update_h) $(object_aspect_visualization_h) $(object_factory_h) $(object_information_dialog_h) $(outfit_ship_dialog_h) $(output_observer_h) $(perspective_h) $(planet_h) $(real_time_h) $(save_game_dialog_h) $(scanner_display_h) $(ship_h) $(ship_class_h) $(shot_h) $(slot_h) $(slot_class_h) $(star_h) $(star_map_display_h) $(state_machine_h) $(states_h) $(storage_h) $(string_cast_h) $(system_h) $(system_statistics_h) $(timeout_notifications_h) $(timing_dialog_h) $(user_interface_h) $(visualizations_h) $(weapon_h) $(weapon_class_h) $(widget_h) $(write_to_xml_stream_h) $(xml_puny_dom_h) $(xml_stream_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

map_dialog.o: map_dialog.cpp $(button_h) $(globals_h) $(key_event_information_h) $(label_h) $(map_dialog_h) $(object_aspect_name_h) $(star_map_display_h) $(system_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

map_knowledge.o: map_knowledge.cpp $(map_knowledge_h) $(system.h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

message.o: message.cpp $(message_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

message_dispatcher.o: message_dispatcher.cpp $(globals_h) $(message_h) $(message_dispatcher_h) $(object_h) $(object_aspect_messages_h) $(referencing_h) $(system_statistics_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

messages.o: messages.cpp $(messages_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

mind.o: mind.cpp $(goal_h) $(mind_h) $(object_aspect_object_container_h) $(state_machine_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

mini_map_display.o: mini_map_display.cpp $(commodity_h) $(mini_map_display_h) $(object_aspect_position_h) $(planet_h) $(ship_h) $(system_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

mouse_button_listener.o: mouse_button_listener.cpp $(mouse_button_listener_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

mouse_motion_listener.o: mouse_motion_listener.cpp $(mouse_motion_listener_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

object.o: object.cpp $(message_h) $(object_h) $(object_aspect_accessory_h) $(object_aspect_messages_h) $(object_aspect_name_h) $(object_aspect_object_container_h) $(object_aspect_outfitting_h) $(object_aspect_physical_h) $(object_aspect_position_h) $(object_aspect_update_h) $(object_aspect_visualization_h) $(real_time_h) $(string_cast_h) $(xml_stream_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

object_aspect_accessory.o: object_aspect_accessory.cpp $(object_aspect_accessory_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

object_aspect_messages.o: object_aspect_messages.cpp $(object_aspect_messages_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

object_aspect_object_container.o: object_aspect_object_container.cpp $(object_h) $(object_aspect_object_container_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

object_aspect_outfitting.o: object_aspect_outfitting.cpp $(object_aspect_outfitting_h) $(slot_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

object_aspect_physical.o: object_aspect_physical.cpp $(object_aspect_physical_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

object_aspect_position.o: object_aspect_position.cpp $(object_aspect_position_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

object_aspect_update.o: object_aspect_update.cpp $(object_aspect_update_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

object_aspect_visualization.o: object_aspect_visualization.cpp $(graphics_node_h) $(object_aspect_visualization_h) $(visualization_prototype_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

object_factory.o: object_factory.cpp $(battery_h) $(battery_class_h) $(character_h) $(class_manager_h) $(command_mind_h) $(commodity_h) $(commodity_class_h) $(faction_h) $(game_time_h) $(generator_h) $(generator_class_h) $(globals_h) $(graphics_model_h) $(object_aspect_name_h) $(object_aspect_outfitting_h) $(object_aspect_physical_h) $(object_aspect_position_h) $(object_aspect_visualization_h) $(object_factory_h) $(planet_h) $(ship_h) $(ship_class_h) $(shot_h) $(slot_h) $(storage_h) $(weapon_h) $(weapon_class_h) $(visualization_prototype_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

object_information_dialog.o: object_information_dialog.cpp $(button_h) $(globals_h) $(label_h) $(object_h) $(object_aspect_accessory_h) $(object_aspect_name_h) $(object_aspect_object_container_h) $(object_aspect_physical_h) $(object_information_dialog_h) $(scroll_bar_h) $(scroll_box_h) $(slot_h) $(slot_class_h) $(string_cast_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

outfit_ship_dialog.o: outfit_ship_dialog.cpp $(button_h) $(class_manager_h) $(color_h) $(globals_h) $(key_event_information_h) $(label_h) $(object_aspect_accessory_h) $(object_aspect_name_h) $(object_aspect_object_container_h) $(object_aspect_outfitting_h) $(object_aspect_physical_h) $(outfit_ship_dialog_h) $(scroll_box_h) $(ship_h) $(slot_h) $(slot_class_h) $(storage_h) $(weapon_h) $(weapon_class_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

output_observer.o: output_observer.cpp $(command_mind_h) $(globals_h) $(message_h) $(output_observer_h) $(planet_h) $(planet_dialog_h) $(user_interface_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

perspective.o: perspective.cpp $(perspective_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

planet.o: planet.cpp $(asset_class_h) $(battery_h) $(color_h) $(globals_h) $(message_h) $(message_dispatcher_h) $(object_aspect_object_container_h) $(object_aspect_physical_h) $(planet_h) $(ship_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

planet_dialog.o: planet_dialog.cpp $(asset_class_h) $(button_h) $(character_h) $(color_h) $(globals_h) $(key_event_information_h) $(label_h) $(object_aspect_name_h) $(planet_h) $(planet_dialog_h) $(ship_h) $(string_cast_h) $(trade_center_dialog_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

real_time.o: real_time.cpp $(real_time_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

save_game_dialog.o: save_game_dialog.cpp $(button_h) $(globals_h) $(key_event_information_h) $(label_h) $(real_time_h) $(save_game_dialog_h) $(scroll_box_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

scanner_display.o: scanner_display.cpp $(object_aspect_physical_h) $(scanner_display_h) $(ship_h) $(star_h) $(system.h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

scroll_bar.o: scroll_bar.cpp $(button_h) $(color_h) $(scroll_bar_h) $(scroll_position_changed_listener_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

scroll_box.o: scroll_box.cpp $(color_h) $(scroll_bar_h) $(scroll_box_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

scroll_position_changed_listener.o: scroll_position_changed_listener.cpp $(scroll_position_changed_listener_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

ship.o: ship.cpp $(battery_h) $(character_h) $(commodity_h) $(game_time_h) $(generator_h) $(globals_h) $(graphics_particle_system_h) $(map_knowledge_h) $(math_h) $(message_h) $(message_dispatcher_h) $(object_aspect_accessory_h) $(object_aspect_object_container_h) $(object_aspect_outfitting_h) $(object_aspect_position_h) $(object_aspect_update_h) $(object_aspect_visualization_h) $(planet_h) $(ship_h) $(slot_h) $(slot_class_h) $(storage_h) $(system_h) $(visualizations_h) $(weapon_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

ship_class.o: ship_class.cpp $(color_h) $(graphics_material_h) $(ship_class_h) $(slot_h) $(visualization_prototype_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

shot.o: shot.cpp $(game_time_h) $(object_aspect_position_h) $(object_aspect_update_h) $(shot_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

slot.o: slot.cpp $(object_h) $(object_aspect_accessory_h) $(slot_h) $(slot_class_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

slot_class.o: slot_class.cpp $(slot_class_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

star.o: star.cpp $(star_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

star_map_display.o: star_map_display.cpp $(character_h) $(draw_text_h) $(galaxy_h) $(globals_h) $(map_knowledge_h) $(object_aspect_name_h) $(object_aspect_position_h) $(star_map_display_h) $(system_h) $(user_interface_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

state.o: state.cpp $(state_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

state_machine.o: state_machine.cpp $(state_h) $(state_machine_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

states.o: states.cpp $(asset_class_h) $(character_h) $(commodity_h) $(commodity_class_h) $(game_time_h) $(globals_h) $(math_h) $(message_h) $(mind_h) $(object_aspect_object_container_h) $(object_aspect_physical_h) $(object_aspect_position_h) $(object_factory_h) $(planet_h) $(ship_h) $(state_machine_h) $(states_h) $(storage_h) $(string_cast_h) $(system_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

storage.o: storage.cpp $(object_aspect_object_container_h) $(object_aspect_physical_h) $(storage_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

string_cast.o: string_cast.cpp $(string_cast_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

system.o: system.cpp $(commodity_h) $(object_aspect_object_container_h) $(planet_h) $(ship_h) $(shot_h) $(star_h) $(system_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

threat.o: threat.cpp $(threat_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

timeout_notifications.o: timeout_notifications.cpp $(timeout_notifications_h) $(type_definitions_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

timing_dialog.o: timing_dialog.cpp $(globals_h) $(label_h) $(string_cast_h) $(system_statistics_h) $(timing_dialog_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

trade_center_dialog.o: trade_center_dialog.cpp $(asset_class_h) $(button_h) $(character_h) $(commodity_h) $(commodity_class_h) $(character_h) $(game_time_h) $(graphics_model_manager_h) $(key_event_information_h) $(label_h) $(object_aspect_accessory_h) $(object_aspect_name_h) $(object_aspect_object_container_h) $(object_factory_h) $(planet_h) $(scroll_bar_h) $(scroll_box_h) $(ship_h) $(storage_h) $(trade_center_dialog_h) $(weapon_h) $(weapon_class_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

user_interface.o: user_interface.cpp $(user_interface_h) $(widget_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

viewport.o: viewport.cpp $(viewport_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

visualization_prototype.o: visualization_prototype.cpp $(graphics_material_h) $(visualization_prototype_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

visualizations.o: visualizations.cpp $(color_h) $(commodity_h) $(commodity_class_h) $(globals_h) $(graphics_material_h) $(graphics_model_h) $(graphics_model_object_h) $(graphics_model_manager_h) $(graphics_particle_system_h) $(object_aspect_accessory_h) $(object_aspect_outfitting_h) $(object_aspect_physical_h) $(object_aspect_position_h) $(object_aspect_visualization_h) $(planet_h) $(ship_h) $(shot_h) $(visualization_prototype_h) $(visualizations_h) $(weapon_h) $(weapon_class_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

weapon.o: weapon.cpp $(battery_h) $(game_time_h) $(globals_h) $(math_h) $(math_quaternion_h) $(object_aspect_accessory_h) $(object_aspect_object_container_h) $(object_aspect_position_h) $(object_factory_h) $(ship_h) $(shot_h) $(slot_h) $(string_cast_h) $(visualizations_h) $(weapon_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

weapon_class.o: weapon_class.cpp $(color_h) $(visualization_prototype_h) $(weapon_class_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

widget.o: widget.cpp $(color_h) $(destroy_listener_h) $(dimension_listener_h) $(key_listener_h) $(math_h) $(mouse_button_listener_h) $(mouse_motion_listener_h) $(widget_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

window.o: window.cpp $(border_h) $(globals_h) $(label_h) $(user_interface_h) $(window_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

write_to_xml_stream.o: write_to_xml_stream.cpp $(battery_h) $(character_h) $(commodity_h) $(generator_h) $(map_knowledge_h) $(mind_h) $(object_h) $(object_aspect_accessory_h) $(object_aspect_name_h) $(object_aspect_object_container_h) $(object_aspect_physical_h) $(object_aspect_position_h) $(ship_h) $(slot_h) $(storage_h) $(system_h) $(weapon_h) $(write_to_xml_stream_h) $(xml_stream_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

xml_parser.o: xml_parser.cpp $(xml_parser_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

xml_puny_dom.o: xml_puny_dom.cpp $(xml_parser_h) $(xml_puny_dom_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

xml_stream.o: xml_stream.cpp $(xml_stream_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@
