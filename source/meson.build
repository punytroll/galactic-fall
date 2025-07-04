arx_types_h = custom_target(
  'arx_types_h',
  capture: true,
  command: [python3_program, generate_cpp_resource_constants_py, '--declarations', declarations_xml],
  output: 'arx_types.h'
)

galactic_fall_executable = executable(
  'galactic_fall',
  sources: [
    arx_types_h,
    'ammunition.cpp',
    'battery.cpp',
    'blueprint.cpp',
    'blueprint_manager.cpp',
    'buffer_reading.cpp',
    'character.cpp',
    'character_observer.cpp',
    'command_mind.cpp',
    'commodity.cpp',
    'energy_projectile_weapon.cpp',
    'faction.cpp',
    'file_handling.cpp',
    'galaxy.cpp',
    'game_saver.cpp',
    'game_time.cpp',
    'generator.cpp',
    'goal.cpp',
    'goal_mind.cpp',
    'goals.cpp',
    'graphics/system_node.cpp',
    'graphics/ui_node.cpp',
    'hangar.cpp',
    'main.cpp',
    'map_knowledge.cpp',
    'message.cpp',
    'message_dispatcher.cpp',
    'messages.cpp',
    'mind.cpp',
    'object.cpp',
    'object_aspect_accessory.cpp',
    'object_aspect_messages.cpp',
    'object_aspect_object_container.cpp',
    'object_aspect_outfitting.cpp',
    'object_aspect_physical.cpp',
    'object_aspect_position.cpp',
    'object_aspect_update.cpp',
    'object_aspect_visualization.cpp',
    'object_factory.cpp',
    'output_observer.cpp',
    'physics/energy/device.cpp',
    'physics/energy/network.cpp',
    'planet.cpp',
    'planet_assets.cpp',
    'properties.cpp',
    'real_time.cpp',
    'resource_reader.cpp',
    'scenario.cpp',
    'scenario_manager.cpp',
    'settings.cpp',
    'ship.cpp',
    'shot.cpp',
    'slot.cpp',
    'slot_class.cpp',
    'star.cpp',
    'state.cpp',
    'state_machine.cpp',
    'states.cpp',
    'storage.cpp',
    'system.cpp',
    'system_statistics.cpp',
    'threat.cpp',
    'timeout_notifications.cpp',
    'turret.cpp',
    'ui/border.cpp',
    'ui/button.cpp',
    'ui/dialog.cpp',
    'ui/dimension.cpp',
    'ui/dimension_operators.cpp',
    'ui/event.cpp',
    'ui/hangar_widget.cpp',
    'ui/heads_up_display.cpp',
    'ui/key_event.cpp',
    'ui/label.cpp',
    'ui/list_box.cpp',
    'ui/list_box_item.cpp',
    'ui/list_box_object_item.cpp',
    'ui/list_box_scenario_item.cpp',
    'ui/list_box_text_item.cpp',
    'ui/load_game_dialog.cpp',
    'ui/load_scenario_dialog.cpp',
    'ui/load_ship_window.cpp',
    'ui/main_menu_window.cpp',
    'ui/map_dialog.cpp',
    'ui/mini_map_display.cpp',
    'ui/mouse_button_event.cpp',
    'ui/mouse_move_event.cpp',
    'ui/object_information_dialog.cpp',
    'ui/outfit_ship_dialog.cpp',
    'ui/planet_window.cpp',
    'ui/progress_bar.cpp',
    'ui/save_game_dialog.cpp',
    'ui/scanner_display.cpp',
    'ui/scroll_bar.cpp',
    'ui/scroll_box.cpp',
    'ui/star_map_display.cpp',
    'ui/sub_widget_event.cpp',
    'ui/text_button.cpp',
    'ui/timing_dialog.cpp',
    'ui/trade_center_widget.cpp',
    'ui/user_interface.cpp',
    'ui/view_display.cpp',
    'ui/widget.cpp',
    'ui/window.cpp',
    'visualization.cpp',
    'visualization_prototype.cpp',
    'visualizations.cpp',
    'weapon.cpp'
  ],
  dependencies: [
    algebra_dependency,
    arxx_dependency,
    events_dependency,
    expressions_dependency,
    graphics_dependency,
    opengl_dependency,
    string_cast_dependency,
    x11_dependency,
    xml_puny_dom_dependency,
    xml_stream_dependency
  ]
)
