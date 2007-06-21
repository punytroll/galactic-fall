arx_reading_h = \
	arx_reading.h

arx_resources_h = \
	arx_resources.h

buffer_reading_h = \
	buffer_reading.h \
	$(color_h)

button_h = \
	button.h \
	$(mouse_button_listener_h) \
	$(mouse_motion_listener_h) \
	$(widget_h)

callbacks_h = \
	callbacks.h

camera_h = \
	camera.h \
	$(position_h)

cargo_h = \
	cargo.h \
	$(physical_object_h)

character_h = \
	character.h \
	$(object_h)

clicked_listener_h = \
	clicked_listener.h

color_h = \
	color.h

command_mind_h = \
	command_mind.h \
	$(mind_h)

commodity_h = \
	commodity.h

commodity_manager_h = \
	commodity_manager.h

destroy_listener_h = \
	destroy_listener.h

draw_text_h = \
	draw_text.h

game_time_h = \
	game_time.h

globals_h = \
	globals.h

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
	$(mouse_button_listener_h) \
	$(window_h)

map_knowledge_h = \
	map_knowledge.h

math_h = \
	math.h

mind_h = \
	mind.h \
	$(object_h)

mini_map_h = \
	mini_map.h \
	$(camera_h) \
	$(perspective_h) \
	$(referencing_h) \
	$(widget_h)

model_h = \
	model.h

model_manager_h = \
	model_manager.h

mouse_button_listener_h = \
	mouse_button_listener.h

mouse_motion_listener_h = \
	mouse_motion_listener.h

object_h = \
	object.h \
	$(referencing_h)

perspective_h = \
	perspective.h

physical_object_h = \
	physical_object.h \
	$(position_h)

planet_h = \
	planet.h \
	$(physical_object_h)

planet_dialog_h = \
	planet_dialog.h \
	$(clicked_listener_h) \
	$(key_listener_h) \
	$(window_h)

position_h = \
	position.h \
	$(object_h)

real_time_h = \
	real_time.h

referencing_h = \
	referencing.h

scanner_display_h = \
	scanner_display.h \
	$(camera_h) \
	$(perspective_h) \
	$(referencing_h) \
	$(widget_h)

ship_h = \
	ship.h \
	$(physical_object_h) \
	$(ship_class_h)

ship_class_h = \
	ship_class.h

ship_class_manager_h = \
	ship_class_manager.h

shot_h = \
	$(physical_object_h) \
	shot.h

star_h = \
	star.h \
	$(position_h)

state_h = \
	state.h

state_machine_h = \
	state_machine.h

states_h = \
	states.h \
	$(state_h)

string_cast_h = \
	string_cast.h

system_h = \
	system.h \
	$(position_h)

system_manager_h = \
	system_manager.h

trade_center_dialog_h = \
	trade_center_dialog.h \
	$(clicked_listener_h) \
	$(key_listener_h) \
	$(mouse_button_listener_h) \
	$(mouse_motion_listener_h) \
	$(window_h)

user_interface_h = \
	user_interface.h

weapon_h = \
	weapon.h \
	$(object_h)

weapon_class_h = \
	weapon_class.h

weapon_class_manager_h = \
	weapon_class_manager.h

widget_h = \
	widget.h \
	$(destroy_listener_h)

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

all: all-recursive

all-recursive: data binary

clean: clean-recursive
	$(RM) galactic-fall
	$(RM) arx_reading.o
	$(RM) buffer_reading.o
	$(RM) button.o
	$(RM) camera.o
	$(RM) cargo.o
	$(RM) character.o
	$(RM) clicked_listener.o
	$(RM) color.o
	$(RM) command_mind.o
	$(RM) commodity.o
	$(RM) commodity_manager.o
	$(RM) destroy_listener.o
	$(RM) draw_text.o
	$(RM) game_time.o
	$(RM) key_listener.o
	$(RM) label.o
	$(RM) main.o
	$(RM) map_dialog.o
	$(RM) map_knowledge.o
	$(RM) mind.o
	$(RM) mini_map.o
	$(RM) model.o
	$(RM) model_manager.o
	$(RM) mouse_button_listener.o
	$(RM) mouse_motion_listener.o
	$(RM) object.o
	$(RM) perspective.o
	$(RM) physical_object.o
	$(RM) planet.o
	$(RM) planet_dialog.o
	$(RM) real_time.o
	$(RM) scanner_display.o
	$(RM) ship.o
	$(RM) ship_class.o
	$(RM) ship_class_manager.o
	$(RM) shot.o
	$(RM) star.o
	$(RM) state.o
	$(RM) state_machine.o
	$(RM) states.o
	$(RM) string_cast.o
	$(RM) system.o
	$(RM) system_manager.o
	$(RM) trade_center_dialog.o
	$(RM) user_interface.o
	$(RM) weapon.o
	$(RM) weapon_class.o
	$(RM) weapon_class_manager.o
	$(RM) widget.o
	$(RM) window.o
	$(RM) xml_parser.o
	$(RM) xml_puny_dom.o
	$(RM) xml_stream.o

clean-recursive:
	@$(MAKE) -C data clean

binary: check-dependencies galactic-fall

data:
	@$(MAKE) -C data all

check-dependencies:
	@if [ x`which pkg-config` = x ]; then \
	    echo -e "\nYour system is missing the \"pkg-config\" tool.\nThis tool is needed and you will need to install it in order to build the program.\n"; false; \
	fi
	@if ! pkg-config --exists libarxx; then \
	    echo -e "\nYour system is missing the \"libarxx\" library.\nYou can get this library from \"http://libarxx.sourceforge.net\"./\n"; false; \
	fi
	@if ! pkg-config --atleast-version=0.7.5 libarxx; then \
	    echo -e "\nThe library \"libarxx\" that is installed on your system is too old.\nYou have: Version `pkg-config --modversion libarxx`\nYou need: Version 0.7.5\nYou can get the most recent library from \"http://libarxx.sourceforge.net\"./\n"; false; \
	fi
	@if ! pkg-config --exists math3d; then \
	    echo -e "\nYour system is missing the \"math3d\" library.\nYou can get this library from the subversion repository at \"http://water-wheel.dyndns.org/subversion/math3d/\".\n"; false; \
	fi
	@if ! pkg-config --atleast-version=0.5 math3d; then \
	    echo -e "\nThe library \"math3d\" that is installed on your system is too old.\nYou have: Version `pkg-config --modversion math3d`\nYou need: Version 0.5\nYou can get the most recent library from the subversion repository at \"http://water-wheel.dyndns.org/subversion/math3d/\".\n"; false; \
	fi

install:
	@echo -e "This project is not installable yet. Please run \"./galactic-fall\" from the top directory."

galactic-fall: arx_reading.o buffer_reading.o button.o camera.o cargo.o character.o clicked_listener.o color.o command_mind.o commodity.o commodity_manager.o destroy_listener.o draw_text.o game_time.o key_listener.o label.o main.o map_dialog.o map_knowledge.o mini_map.o mind.o model.o model_manager.o mouse_button_listener.o mouse_motion_listener.o object.o perspective.o physical_object.o planet.o planet_dialog.o real_time.o scanner_display.o ship.o ship_class.o ship_class_manager.o shot.o star.o state.o state_machine.o states.o string_cast.o system.o system_manager.o trade_center_dialog.o user_interface.o weapon.o weapon_class.o weapon_class_manager.o widget.o window.o xml_parser.o xml_puny_dom.o xml_stream.o
	$(CXX) $(LDFLAGS) `pkg-config --libs libarxx` $^ -lGL -o $@

arx_reading.o: arx_reading.cpp $(arx_reading_h) $(arx_resources_h) $(buffer_reading_h) $(callbacks_h) $(commodity_h) $(commodity_manager_h) $(label_h) $(model_h) $(model_manager_h) $(planet_h) $(ship_class_h) $(ship_class_manager_h) $(system_h) $(system_manager_h) $(user_interface_h) $(weapon_class_h) $(weapon_class_manager_h) $(widget_h)
	$(CXX) $(CXXFLAGS) `pkg-config --cflags libarxx math3d` -c $< -o $@

buffer_reading.o: buffer_reading.cpp $(buffer_reading_h)
	$(CXX) $(CXXFLAGS) `pkg-config --cflags libarxx math3d` -c $< -o $@

button.o: button.cpp $(button_h) $(clicked_listener_h) $(color_h) $(globals_h)
	$(CXX) $(CXXFLAGS) `pkg-config --cflags math3d` -c $< -o $@

camera.o: camera.cpp $(camera_h) $(position_h)
	$(CXX) $(CXXFLAGS) `pkg-config --cflags math3d` -c $< -o $@

cargo.o: cargo.cpp $(cargo_h) $(color_h) $(commodity_h) $(model_h)
	$(CXX) $(CXXFLAGS) `pkg-config --cflags math3d` -c $< -o $@

character.o: character.cpp $(character_h) $(map_knowledge_h) $(mind_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clicked_listener.o: clicked_listener.cpp $(clicked_listener_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

color.o: color.cpp $(color_h)
	$(CXX) $(CXXFLAGS) `pkg-config --cflags math3d` -c $< -o $@

command_mind.o: command_mind.cpp $(cargo_h) $(character_h) $(command_mind_h) $(planet_h) $(ship_h) $(system_h)
	$(CXX) $(CXXFLAGS) `pkg-config --cflags math3d` -c $< -o $@

commodity.o: commodity.cpp $(color_h) $(commodity_h)
	$(CXX) $(CXXFLAGS) `pkg-config --cflags math3d` -c $< -o $@

commodity_manager.o: commodity_manager.cpp $(commodity_h) $(commodity_manager_h) $(string_cast_h) $(xml_puny_dom_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

destroy_listener.o: destroy_listener.cpp $(destroy_listener_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

draw_text.o: draw_text.cpp $(draw_text_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

game_time.o: game_time.cpp $(game_time_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

key_listener.o: key_listener.cpp $(key_listener_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

label.o: label.cpp $(label_h)
	$(CXX) $(CXXFLAGS) `pkg-config --cflags math3d` -c $< -o $@

main.o: main.cpp $(arx_resources_h) $(arx_reading_h) $(callbacks_h) $(camera_h) $(cargo_h) $(character_h) $(color_h) $(command_mind_h) $(commodity_h) $(commodity_manager_h) $(destroy_listener_h) $(game_time_h) $(label_h) $(map_dialog_h) $(map_knowledge_h) $(math_h) $(mind_h) $(mini_map_h) $(model_h) $(model_manager_h) $(perspective_h) $(planet_h) $(planet_dialog_h) $(real_time_h) $(scanner_display_h) $(ship_h) $(ship_class_h) $(ship_class_manager_h) $(star_h) $(state_machine_h) $(states_h) $(string_cast_h) $(system_h) $(system_manager_h) $(user_interface_h) $(weapon_h) $(weapon_class_h) $(weapon_class_manager_h) $(widget_h) $(xml_puny_dom_h) $(xml_stream_h)
	$(CXX) $(CXXFLAGS) `pkg-config --cflags libarxx math3d` -c $< -o $@

map_dialog.o: map_dialog.cpp $(button_h) $(character_h) $(color_h) $(globals_h) $(label) $(map_dialog_h) $(map_knowledge_h) $(system_h) $(system_manager_h)
	$(CXX) $(CXXFLAGS) `pkg-config --cflags math3d` -c $< -o $@

map_knowledge.o: map_knowledge.cpp $(map_knowledge_h) $(system.h)
	$(CXX) $(CXXFLAGS) `pkg-config --cflags math3d` -c $< -o $@

mind.o: mind.cpp $(mind_h) $(state_machine_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

mini_map.o: mini_map.cpp $(cargo_h) $(mini_map_h) $(planet_h) $(ship_h) $(system_h)
	$(CXX) $(CXXFLAGS) `pkg-config --cflags math3d` -c $< -o $@

model.o: model.cpp $(model_h)
	$(CXX) $(CXXFLAGS) `pkg-config --cflags math3d` -c $< -o $@

model_manager.o: model_manager.cpp $(model_h) $(model_manager_h) $(string_cast_h) $(xml_puny_dom_h)
	$(CXX) $(CXXFLAGS) `pkg-config --cflags math3d` -c $< -o $@

mouse_button_listener.o: mouse_button_listener.cpp $(mouse_button_listener_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

mouse_motion_listener.o: mouse_motion_listener.cpp $(mouse_motion_listener_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

object.o: object.cpp $(object_h) $(real_time_h) $(string_cast_h) $(xml_stream_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

perspective.o: perspective.cpp $(perspective_h)
	$(CXX) $(CXXFLAGS) `pkg-config --cflags math3d` -c $< -o $@

physical_object.o: physical_object.cpp $(physical_object_h)
	$(CXX) $(CXXFLAGS) `pkg-config --cflags math3d` -c $< -o $@

planet.o: planet.cpp $(color_h) $(commodity_h) $(globals_h) $(model_h) $(model_manager_h) $(planet_h)
	$(CXX) $(CXXFLAGS) `pkg-config --cflags math3d` -c $< -o $@

planet_dialog.o: planet_dialog.cpp $(button_h) $(character_h) $(color_h) $(globals_h) $(label_h) $(planet_h) $(planet_dialog_h) $(ship_h) $(string_cast_h) $(trade_center_dialog_h)
	$(CXX) $(CXXFLAGS) `pkg-config --cflags math3d` -c $< -o $@

real_time.o: real_time.cpp $(real_time_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

scanner_display.o: scanner_display.cpp $(scanner_display_h) $(ship_h) $(star_h) $(system.h)
	$(CXX) $(CXXFLAGS) `pkg-config --cflags math3d` -c $< -o $@

ship.o: ship.cpp $(cargo_h) $(character_h) $(color_h) $(commodity_h) $(game_time_h) $(globals_h) $(map_knowledge_h) $(math_h) $(model_h) $(model_manager_h) $(ship_h) $(shot_h) $(string_cast_h) $(system_h) $(weapon_h)
	$(CXX) $(CXXFLAGS) `pkg-config --cflags math3d` -c $< -o $@

ship_class.o: ship_class.cpp $(color_h) $(ship_class_h)
	$(CXX) $(CXXFLAGS) `pkg-config --cflags math3d` -c $< -o $@
	
ship_class_manager.o: ship_class_manager.cpp $(ship_class_h) $(ship_class_manager_h)
	$(CXX) $(CXXFLAGS) `pkg-config --cflags math3d` -c $< -o $@

shot.o: shot.cpp $(color_h) $(game_time_h) $(shot_h) $(weapon_class_h)
	$(CXX) $(CXXFLAGS) `pkg-config --cflags math3d` -c $< -o $@

star.o: star.cpp $(star_h)
	$(CXX) $(CXXFLAGS) `pkg-config --cflags math3d` -c $< -o $@

state.o: state.cpp $(state_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

state_machine.o: state_machine.cpp $(state_h) $(state_machine_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

states.o: states.cpp $(cargo_h) $(character_h) $(commodity) $(game_time_h) $(math_h) $(mind_h) $(planet_h) $(ship_h) $(state_machine_h) $(states_h) $(string_cast_h) $(system_h)
	$(CXX) $(CXXFLAGS) `pkg-config --cflags math3d` -c $< -o $@

string_cast.o: string_cast.cpp $(string_cast_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

system.o: system.cpp $(planet_h) $(star_h) $(system_h)
	$(CXX) $(CXXFLAGS) `pkg-config --cflags math3d` -c $< -o $@

system_manager.o: system_manager.cpp $(system_h) $(system_manager_h)
	$(CXX) $(CXXFLAGS) `pkg-config --cflags math3d` -c $< -o $@

trade_center_dialog.o: trade_center_dialog.cpp $(button_h) $(cargo_h) $(character_h) $(commodity_h) $(model_manager_h) $(label_h) $(planet_h) $(real_time_h) $(ship_h) $(trade_center_dialog_h)
	$(CXX) $(CXXFLAGS) `pkg-config --cflags math3d` -c $< -o $@

user_interface.o: user_interface.cpp $(user_interface_h) $(widget_h)
	$(CXX) $(CXXFLAGS) `pkg-config --cflags math3d` -c $< -o $@

weapon.o: weapon.cpp $(game_time_h) $(ship_h) $(shot_h) $(string_cast_h) $(system_h) $(weapon_h) $(weapon_class_h)
	$(CXX) $(CXXFLAGS) `pkg-config --cflags math3d` -c $< -o $@

weapon_class.o: weapon_class.cpp $(color_h) $(weapon_class_h)
	$(CXX) $(CXXFLAGS) `pkg-config --cflags math3d` -c $< -o $@

weapon_class_manager.o: weapon_class_manager.cpp $(weapon_class_h) $(weapon_class_manager_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

widget.o: widget.cpp $(color_h) $(key_listener_h) $(mouse_button_listener_h) $(mouse_motion_listener_h) $(widget_h)
	$(CXX) $(CXXFLAGS) `pkg-config --cflags math3d` -c $< -o $@

window.o: window.cpp $(globals_h) $(label_h) $(user_interface_h) $(window_h)
	$(CXX) $(CXXFLAGS) `pkg-config --cflags math3d` -c $< -o $@

xml_parser.o: xml_parser.cpp $(xml_parser_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

xml_puny_dom.o: xml_puny_dom.cpp $(xml_parser_h) $(xml_puny_dom_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

xml_stream.o: xml_stream.cpp $(xml_stream_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

.PHONY: data
