SOURCE_FILES = $(wildcard *.cpp)
OBJECT_FILES = $(patsubst %.cpp,%.o,$(SOURCE_FILES))
HEADER_DEPENDENCIES = $(foreach HEADER_FILE, $(wildcard *.h), $(HEADER_FILE) '$($(patsubst %.h,%_h,$(HEADER_FILE)))')

arx_reading_h = \
	arx_reading.h

arx_resources_h = \
	arx_resources.h

asset_class_h = \
	asset_class.h

asset_class_manager_h = \
	asset_class_manager.h

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

callbacks_h = \
	callbacks.h

camera_h = \
	camera.h \
	$(math_vector3f_h) \
	$(position_h)

character_h = \
	character.h \
	$(object_h) \
	$(type_definitions_h)

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
	$(math_quaternion_h) \
	$(math_vector3f_h) \
	$(physical_object_h)

commodity_class_h = \
	commodity_class.h \
	$(type_definitions_h)

commodity_class_manager_h = \
	commodity_class_manager.h

destroy_listener_h = \
	destroy_listener.h

draw_text_h = \
	draw_text.h

file_handling_h = \
	file_handling.h

galaxy_h = \
	galaxy.h \
	$(object_h)

game_time_h = \
	game_time.h

globals_h = \
	globals.h

graphics_engine_h = \
	graphics_engine.h

graphics_node_h = \
	graphics_node.h \
	$(math_quaternion_h) \
	$(math_vector3f_h)

graphics_particle_systems_h = \
	graphics_particle_systems.h \
	$(color_h) \
	$(graphics_node_h) \
	$(math_vector2f_h)

key_event_information_h = \
	key_event_information.h

key_listener_h = \
	key_listener.h

label_h = \
	label.h \
	$(color_h) \
	$(widget_h)

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

mind_h = \
	mind.h \
	$(object_h)

mini_map_display_h = \
	mini_map_display.h \
	$(referencing_h) \
	$(widget_h)

model_h = \
	model.h \
	$(math_vector4f_h)

model_manager_h = \
	model_manager.h

mount_weapon_dialog_h = \
	mount_weapon_dialog.h \
	$(clicked_listener_h) \
	$(key_listener_h) \
	$(mouse_button_listener_h) \
	$(window_h)

mouse_button_listener_h = \
	mouse_button_listener.h

mouse_motion_listener_h = \
	mouse_motion_listener.h

object_h = \
	object.h \
	$(referencing_h)

object_factory_h = \
	object_factory.h

perspective_h = \
	perspective.h \
	$(math_vector4f_h)

physical_object_h = \
	physical_object.h \
	$(position_h) \
	$(type_definitions_h)

planet_h = \
	planet.h \
	$(physical_object_h) \
	$(type_definitions_h)

planet_dialog_h = \
	planet_dialog.h \
	$(clicked_listener_h) \
	$(key_listener_h) \
	$(window_h)

position_h = \
	position.h \
	$(math_vector3f_h) \
	$(object_h)

real_time_h = \
	real_time.h

referencing_h = \
	referencing.h

save_game_dialog_h = \
	save_game_dialog.h \
	$(callbacks_h) \
	$(clicked_listener_h) \
	$(key_listener_h) \
	$(mouse_button_listener_h) \
	$(window_h)

scanner_display_h = \
	scanner_display.h \
	$(referencing_h) \
	$(viewport_h)

scroll_bar_h = \
	scroll_bar.h \
	$(clicked_listener_h) \
	$(widget_h)

scroll_box_h = \
	scroll_box.h \
	$(scroll_position_changed_listener_h) \
	$(widget_h)

scroll_position_changed_listener_h = \
	scroll_position_changed_listener.h \

ship_h = \
	ship.h \
	$(math_vector2f_h) \
	$(physical_object_h) \
	$(ship_class_h) \
	$(type_definitions_h)

ship_class_h = \
	ship_class.h \
	$(math_vector3f_h) \
	$(type_definitions_h)

ship_class_manager_h = \
	ship_class_manager.h

shot_h = \
	shot.h \
	$(math_quaternion_h) \
	$(math_vector3f_h) \
	$(physical_object_h)

slot_h = \
	slot.h \
	$(math_quaternion_h) \
	$(math_vector3f_h) \
	$(referencing_h)

slot_class_h = \
	slot_class.h

slot_class_manager_h = \
	slot_class_manager.h

star_h = \
	star.h \
	$(position_h)

star_map_display_h = \
	star_map_display.h \
	$(mouse_button_listener_h) \
	$(mouse_motion_listener_h) \
	$(widget_h)

state_h = \
	state.h

state_machine_h = \
	state_machine.h

states_h = \
	states.h \
	$(math_vector3f_h) \
	$(state_h)

string_cast_h = \
	string_cast.h

system_h = \
	system.h \
	$(position_h)

timeout_notifications_h = \
	timeout_notifications.h \
	$(callbacks_h)

timing_dialog_h = \
	timing_dialog.h \
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

weapon_h = \
	weapon.h \
	$(math_quaternion_h) \
	$(physical_object_h)

weapon_class_h = \
	weapon_class.h \
	$(math_quaternion_h) \
	$(math_vector3f_h) \
	$(type_definitions_h)

weapon_class_manager_h = \
	weapon_class_manager.h

widget_h = \
	widget.h \
	$(destroy_listener_h) \
	$(math_vector2f_h)

window_h = \
	window.h \
	$(mouse_button_listener_h) \
	$(mouse_motion_listener_h) \
	$(widget_h)

xml_parser_h = \
	xml_parser.h

xml_puny_dom_h = \
	xml_puny_dom.h

xml_stream_h = \
	xml_stream.h

all: check-dependencies check-consistency binary

clean:
	$(RM) binary
	$(RM) $(OBJECT_FILES)

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
	for HEADER_DEPENDENCY in $(HEADER_DEPENDENCIES); do \
	    if [ -z $${HEADER_FILE} ]; then \
		HEADER_FILE=$${HEADER_DEPENDENCY}; \
	    else \
		if [ -z "$${HEADER_DEPENDENCY}" ]; then \
		    echo -e "\nThe build system contains no dependency specification for the header file '"$${HEADER_FILE}"'.\n"; \
		    exit 1; \
		else \
		    for HEADER_FILE_DEPENDENCY in $${HEADER_DEPENDENCY}; do \
			if [ ! -f $${HEADER_FILE_DEPENDENCY} ]; then \
			    echo -e "\nThe build system contains a reference to the non-existent file '"$${HEADER_FILE_DEPENDENCY}"' for the header file '"$${HEADER_FILE}"'.\n"; \
			    exit 1; \
			fi; \
		    done; \
		fi; \
		HEADER_FILE=; \
	    fi \
	done

binary: $(OBJECT_FILES)
	$(CXX) $(LDFLAGS) `pkg-config --libs libarxx` $^ -lGL -o $@

arx_reading.o: arx_reading.cpp $(arx_reading_h) $(arx_resources_h) $(asset_class_h) $(asset_class_manager_h) $(buffer_reading_h) $(callbacks_h) $(commodity_class_h) $(commodity_class_manager_h) $(galaxy_h) $(globals_h) $(label_h) $(mini_map_display_h) $(model_h) $(model_manager_h) $(planet_h) $(scanner_display_h) $(ship_class_h) $(ship_class_manager_h) $(slot_h) $(star_h) $(string_cast_h) $(system_h) $(user_interface_h) $(weapon_class_h) $(weapon_class_manager_h) $(widget_h)
	$(CXX) $(CXXFLAGS) `pkg-config --cflags libarxx` -c $< -o $@

asset_class.o: asset_class.cpp $(asset_class_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

asset_class_manager.o: asset_class_manager.cpp $(asset_class_h) $(asset_class_manager_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

border.o: border.cpp $(border_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

buffer_reading.o: buffer_reading.cpp $(buffer_reading_h)
	$(CXX) $(CXXFLAGS) `pkg-config --cflags libarxx` -c $< -o $@

button.o: button.cpp $(button_h) $(clicked_listener_h) $(color_h) $(globals_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

camera.o: camera.cpp $(camera_h) $(position_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

character.o: character.cpp $(character_h) $(map_knowledge_h) $(mind_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clicked_listener.o: clicked_listener.cpp $(clicked_listener_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

color.o: color.cpp $(color_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

command_mind.o: command_mind.cpp $(commodity_h) $(character_h) $(command_mind_h) $(planet_h) $(ship_h) $(system_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

commodity.o: commodity.cpp $(commodity_h) $(color_h) $(commodity_class_h) $(math_matrix4f_h) $(model_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

commodity_class.o: commodity_class.cpp $(color_h) $(commodity_class_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

commodity_class_manager.o: commodity_class_manager.cpp $(commodity_class_h) $(commodity_class_manager_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

destroy_listener.o: destroy_listener.cpp $(destroy_listener_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

draw_text.o: draw_text.cpp $(draw_text_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

file_handling.o: file_handling.cpp $(file_handling_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

galaxy.o: galaxy.cpp $(galaxy_h) $(system_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

game_time.o: game_time.cpp $(game_time_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

graphics_engine.o: graphics_engine.cpp $(graphics_engine_h) $(graphics_node_h) $(graphics_particle_systems_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

graphics_node.o: graphics_node.cpp $(graphics_node_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

graphics_particle_systems.o: graphics_particle_systems.cpp $(color_h) $(game_time_h) $(graphics_particle_systems_h) $(math_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

key_event_information.o: key_event_information.cpp $(key_event_information_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

key_listener.o: key_listener.cpp $(key_listener_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

label.o: label.cpp $(label_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

main.o: main.cpp $(arx_resources_h) $(arx_reading_h) $(asset_class_h) $(asset_class_manager_h) $(callbacks_h) $(camera_h) $(commodity_h) $(character_h) $(color_h) $(command_mind_h) $(destroy_listener_h) $(galaxy_h) $(game_time_h) $(graphics_engine_h) $(graphics_particle_systems_h) $(key_event_information_h) $(label_h) $(map_dialog_h) $(map_knowledge_h) $(math_h) $(mind_h) $(mini_map_display_h) $(model_h) $(model_manager_h) $(mount_weapon_dialog_h) $(object_factory_h) $(perspective_h) $(planet_h) $(planet_dialog_h) $(real_time_h) $(save_game_dialog_h) $(scanner_display_h) $(ship_h) $(ship_class_h) $(ship_class_manager_h) $(shot_h) $(slot_h) $(slot_class_manager_h) $(star_h) $(star_map_display_h) $(state_machine_h) $(states_h) $(string_cast_h) $(system_h) $(timeout_notifications_h) $(timing_dialog_h) $(user_interface_h) $(weapon_h) $(weapon_class_h) $(weapon_class_manager_h) $(widget_h) $(xml_puny_dom_h) $(xml_stream_h)
	$(CXX) $(CXXFLAGS) `pkg-config --cflags libarxx` -c $< -o $@

map_dialog.o: map_dialog.cpp $(button_h) $(globals_h) $(key_event_information_h) $(label) $(map_dialog_h) $(star_map_display_h) $(system_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

map_knowledge.o: map_knowledge.cpp $(map_knowledge_h) $(system.h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

mind.o: mind.cpp $(mind_h) $(state_machine_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

mini_map_display.o: mini_map_display.cpp $(commodity_h) $(mini_map_display_h) $(planet_h) $(ship_h) $(system_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

model.o: model.cpp $(model_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

model_manager.o: model_manager.cpp $(model_h) $(model_manager_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

mount_weapon_dialog.o: mount_weapon_dialog.cpp $(button_h) $(color_h) $(globals_h) $(key_event_information_h) $(label_h) $(mount_weapon_dialog_h) $(ship_h) $(slot_h) $(slot_class_h) $(slot_class_manager_h) $(weapon_h) $(weapon_class_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

mouse_button_listener.o: mouse_button_listener.cpp $(mouse_button_listener_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

mouse_motion_listener.o: mouse_motion_listener.cpp $(mouse_motion_listener_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

object.o: object.cpp $(object_h) $(real_time_h) $(string_cast_h) $(xml_stream_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

object_factory.o: object_factory.cpp $(commodity_h) $(commodity_class_h) $(commodity_class_manager_h) $(globals_h) $(object_factory_h) $(weapon_h) $(weapon_class_h) $(weapon_class_manager_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

perspective.o: perspective.cpp $(perspective_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

physical_object.o: physical_object.cpp $(physical_object_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

planet.o: planet.cpp $(color_h) $(asset_class_h) $(globals_h) $(model_h) $(model_manager_h) $(planet_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

planet_dialog.o: planet_dialog.cpp $(asset_class_h) $(button_h) $(character_h) $(color_h) $(globals_h) $(key_event_information_h) $(label_h) $(planet_h) $(planet_dialog_h) $(ship_h) $(string_cast_h) $(trade_center_dialog_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

real_time.o: real_time.cpp $(real_time_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

save_game_dialog.o: save_game_dialog.cpp $(button_h) $(globals_h) $(key_event_information_h) $(label_h) $(real_time_h) $(save_game_dialog_h) $(scroll_box_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

scanner_display.o: scanner_display.cpp $(scanner_display_h) $(ship_h) $(star_h) $(system.h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

scroll_bar.o: scroll_bar.cpp $(button_h) $(color_h) $(scroll_bar_h) $(scroll_position_changed_listener_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

scroll_box.o: scroll_box.cpp $(color_h) $(scroll_box_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

scroll_position_changed_listener.o: scroll_position_changed_listener.cpp $(scroll_position_changed_listener_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

ship.o: ship.cpp $(commodity_h) $(character_h) $(color_h) $(commodity_class_h) $(game_time_h) $(globals_h) $(map_knowledge_h) $(math_h) $(model_h) $(model_manager_h) $(ship_h) $(shot_h) $(slot_h) $(slot_class_h) $(string_cast_h) $(system_h) $(weapon_h) $(weapon_class_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

ship_class.o: ship_class.cpp $(color_h) $(ship_class_h) $(slot_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@
	
ship_class_manager.o: ship_class_manager.cpp $(ship_class_h) $(ship_class_manager_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

shot.o: shot.cpp $(color_h) $(game_time_h) $(math_matrix4f_h) $(model_h) $(shot_h) $(weapon_class_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

slot.o: slot.cpp $(slot_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

slot_class.o: slot_class.cpp $(slot_class_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

slot_class_manager.o: slot_class_manager.cpp $(slot_class_h) $(slot_class_manager_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

star.o: star.cpp $(star_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

star_map_display.o: star_map_display.cpp $(character_h) $(draw_text_h) $(galaxy_h) $(globals_h) $(map_knowledge_h) $(star_map_display_h) $(system_h) $(user_interface_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

state.o: state.cpp $(state_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

state_machine.o: state_machine.cpp $(state_h) $(state_machine_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

states.o: states.cpp $(asset_class_h) $(character_h) $(commodity_h) $(commodity_class_h) $(game_time_h) $(globals_h) $(math_h) $(mind_h) $(object_factory_h) $(planet_h) $(ship_h) $(state_machine_h) $(states_h) $(string_cast_h) $(system_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

string_cast.o: string_cast.cpp $(string_cast_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

system.o: system.cpp $(commodity_h) $(planet_h) $(ship_h) $(shot_h) $(star_h) $(system_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

timeout_notifications.o: timeout_notifications.cpp $(timeout_notifications_h) $(type_definitions_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

timing_dialog.o: timing_dialog.cpp $(label_h) $(string_cast_h) $(timing_dialog_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

trade_center_dialog.o: trade_center_dialog.cpp $(asset_class_h) $(button_h) $(commodity_h) $(commodity_class_h) $(character_h) $(game_time_h) $(key_event_information_h) $(model_manager_h) $(label_h) $(object_factory_h) $(planet_h) $(scroll_bar_h) $(scroll_box_h) $(ship_h) $(trade_center_dialog_h) $(weapon_h) $(weapon_class_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

user_interface.o: user_interface.cpp $(user_interface_h) $(widget_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

viewport.o: viewport.cpp $(viewport_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

weapon.o: weapon.cpp $(color_h) $(game_time_h) $(math_matrix4f_h) $(math_quaternion_h) $(math_vector4f_h) $(model_h) $(ship_h) $(shot_h) $(slot_h) $(string_cast_h) $(system_h) $(weapon_h) $(weapon_class_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

weapon_class.o: weapon_class.cpp $(color_h) $(weapon_class_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

weapon_class_manager.o: weapon_class_manager.cpp $(weapon_class_h) $(weapon_class_manager_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

widget.o: widget.cpp $(color_h) $(key_listener_h) $(math_h) $(mouse_button_listener_h) $(mouse_motion_listener_h) $(widget_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

window.o: window.cpp $(border_h) $(globals_h) $(label_h) $(user_interface_h) $(window_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

xml_parser.o: xml_parser.cpp $(xml_parser_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

xml_puny_dom.o: xml_puny_dom.cpp $(xml_parser_h) $(xml_puny_dom_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

xml_stream.o: xml_stream.cpp $(xml_stream_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@
