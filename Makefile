button_h = \
	button.h \
	$(mouse_button_listener_h) \
	$(widget_h)

clicked_listener_h = \
	clicked_listener.h

color_h = \
	color.h

commodity_h = \
	commodity.h

commodity_manager_h = \
	commodity_manager.h

destroy_listener_h = \
	destroy_listener.h

draw_text_h = \
	draw_text.h

globals_h = \
	globals.h

label_h = \
	label.h \
	$(color_h) \
	$(widget_h)

model_h = \
	model.h

model_manager_h = \
	model_manager.h

mouse_button_listener_h = \
	mouse_button_listener.h

planet_h = \
	planet.h \
	$(position_h)

planet_dialog_h = \
	planet_dialog.h \
	$(clicked_listener_h) \
	$(destroy_listener_h) \
	$(widget_h)

position_h = \
	position.h

ship_h = \
	ship.h \
	$(position_h)

ship_class_h = \
	ship_class.h

ship_class_manager_h = \
	ship_class_manager.h

string_cast_h = \
	string_cast.h

system_h = \
	system.h

system_manager_h = \
	system_manager.h

trade_center_dialog_h = \
	trade_center_dialog.h \
	$(clicked_listener_h) \
	$(mouse_button_listener_h) \
	$(widget_h)

widget_h = \
	widget.h

xml_parser_h = \
	xml_parser.h

xml_puny_dom_h = \
	xml_puny_dom.h

all: escapevelocity

clean:
	$(RM) escapevelocity
	$(RM) button.o
	$(RM) character.o
	$(RM) clicked_listener.o
	$(RM) color.o
	$(RM) commodity.o
	$(RM) commodity_manager.o
	$(RM) destroy_listener.o
	$(RM) draw_text.o
	$(RM) label.o
	$(RM) main.o
	$(RM) model.o
	$(RM) model_manager.o
	$(RM) mouse_button_listener.o
	$(RM) planet.o
	$(RM) planet_dialog.o
	$(RM) ship.o
	$(RM) ship_class.o
	$(RM) ship_class_manager.o
	$(RM) string_cast.o
	$(RM) system.o
	$(RM) system_manager.o
	$(RM) trade_center_dialog.o
	$(RM) widget.o
	$(RM) xml_parser.o
	$(RM) xml_puny_dom.o

escapevelocity: button.o character.o clicked_listener.o color.o commodity.o commodity_manager.o destroy_listener.o draw_text.o label.o main.o model.o model_manager.o mouse_button_listener.o planet.o planet_dialog.o ship.o ship_class.o ship_class_manager.o string_cast.o system.o system_manager.o trade_center_dialog.o widget.o xml_parser.o xml_puny_dom.o
	$(CXX) $(LDFLAGS) $^ -lGL -lGLU -lglut -o $@

button.o: button.cpp $(clicked_listener_h) $(button_h)
	$(CXX) $(CXXFLAGS) `pkg-config --cflags math3d` -c $< -o $@

character.o: character.cpp $(character_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clicked_listener.o: clicked_listener.cpp $(clicked_listener_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

color.o: color.cpp $(color_h)
	$(CXX) $(CXXFLAGS) `pkg-config --cflags math3d` -c $< -o $@

commodity.o: commodity.cpp $(commodity_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

commodity_manager.o: commodity_manager.cpp $(commodity_h) $(commodity_manager_h) $(string_cast_h) $(xml_puny_dom_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

destroy_listener.o: destroy_listener.cpp $(destroy_listener_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

draw_text.o: draw_text.cpp $(draw_text_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

label.o: label.cpp $(label_h)
	$(CXX) $(CXXFLAGS) `pkg-config --cflags math3d` -c $< -o $@

main.o: main.cpp $(character_h) $(color_h) $(commodity_manager_h) $(destroy_listener_h) $(label_h) $(model_h) $(model_manager_h) $(planet_h) $(planet_dialog_h) $(ship_h) $(ship_class_h) $(ship_class_manager_h) $(system_h) $(system_manager_h) $(widget_h)
	$(CXX) $(CXXFLAGS) `pkg-config --cflags math3d` -c $< -o $@

model.o: model.cpp $(model_h)
	$(CXX) $(CXXFLAGS) `pkg-config --cflags math3d` -c $< -o $@

model_manager.o: model_manager.cpp $(model_h) $(model_manager_h) $(string_cast_h) $(xml_puny_dom_h)
	$(CXX) $(CXXFLAGS) `pkg-config --cflags math3d` -c $< -o $@

mouse_button_listener.o: mouse_button_listener.cpp $(mouse_button_listener_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

planet.o: planet.cpp $(commodity_h) $(planet_h)
	$(CXX) $(CXXFLAGS) `pkg-config --cflags math3d` -c $< -o $@

planet_dialog.o: planet_dialog.cpp $(button_h) $(color_h) $(label_h) $(planet_h) $(planet_dialog_h) $(trade_center_dialog_h)
	$(CXX) $(CXXFLAGS) `pkg-config --cflags math3d` -c $< -o $@

ship.o: ship.cpp $(ship_h) $(ship_class_h)
	$(CXX) $(CXXFLAGS) `pkg-config --cflags math3d` -c $< -o $@

ship_class.o: ship_class.cpp $(ship_class_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@
	
ship_class_manager.o: ship_class_manager.cpp $(model_manager_h) $(ship_class_h) $(ship_class_manager_h) $(string_cast_h) $(xml_puny_dom_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

string_cast.o: string_cast.cpp $(string_cast_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

system.o: system.cpp $(system_h)
	$(CXX) $(CXXFLAGS) `pkg-config --cflags math3d` -c $< -o $@

system_manager.o: system_manager.cpp $(commodity_manager_h) $(planet_h) $(string_cast_h) $(system_h) $(system_manager_h) $(xml_puny_dom_h)
	$(CXX) $(CXXFLAGS) `pkg-config --cflags math3d` -c $< -o $@

trade_center_dialog.o: trade_center_dialog.cpp $(button_h) $(commodity_h) $(globals_h) $(label_h) $(planet_h) $(ship_h) $(trade_center_dialog_h)
	$(CXX) $(CXXFLAGS) `pkg-config --cflags math3d` -c $< -o $@

widget.o: widget.cpp $(color_h) $(destroy_listener_h) $(mouse_button_listener_h) $(widget_h)
	$(CXX) $(CXXFLAGS) `pkg-config --cflags math3d` -c $< -o $@

xml_parser.o: xml_parser.cpp $(xml_parser_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

xml_puny_dom.o: xml_puny_dom.cpp $(xml_parser_h) $(xml_puny_dom_h)
	$(CXX) $(CXXFLAGS) -c $< -o $@
