graphics_library = static_library(
  'graphics',
  sources: [
    'source/callback_node.cpp',
    'source/camera.cpp',
    'source/color_rgb.cpp',
    'source/color_rgbo.cpp',
    'source/default_render_target.cpp',
    'source/drawing.cpp',
    'source/engine.cpp',
    'source/gl.cpp',
    'source/light.cpp',
    'source/mesh_manager.cpp',
    'source/mesh_node.cpp',
    'source/mesh.cpp',
    'source/model_manager.cpp',
    'source/model.cpp',
    'source/node.cpp',
    'source/orthogonal_2d_projection.cpp',
    'source/particle_system_node.cpp',
    'source/particle_system.cpp',
    'source/perspective_projection.cpp',
    'source/program.cpp',
    'source/projection.cpp',
    'source/render_context.cpp',
    'source/render_target.cpp',
    'source/scene.cpp',
    'source/shader.cpp',
    'source/shading_manager.cpp',
    'source/style.cpp',
    'source/texture_manager.cpp',
    'source/texture_render_target.cpp',
    'source/texture.cpp',
    'source/view.cpp',
  ],
  dependencies: [
    algebra_dependency
  ],
  include_directories: [include_directories('include')]
)

graphics_dependency = declare_dependency(
  include_directories: [include_directories('include')],
  link_with: [graphics_library]
)
