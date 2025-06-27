project(
  'galactic-fall',
  'cpp',
  version: '0.1',
  meson_version: '>=1.8.2',
  default_options: {
    'cpp_std': 'c++20',
    'default_library': 'static',
    'warning_level': '3',
    'werror': 'true'
  }
)

python3_program = find_program('python3')

algebra_subproject = subproject('algebra', version: '>=2.1.1')
algebra_dependency = algebra_subproject.get_variable('algebra_library_dependency')

expressions_subproject = subproject('expressions', version: '>=0.7.1')
expressions_dependency = expressions_subproject.get_variable('expressions_library_dependency')

string_cast_subproject = subproject('string_cast', version: '>=2.0.2')
string_cast_dependency = string_cast_subproject.get_variable('string_cast_library_dependency')

xml_parser_subproject = subproject('xml_parser', version: '>=2.0.2')
xml_parser_dependency = xml_parser_subproject.get_variable('xml_parser_library_dependency')

xml_puny_dom_subproject = subproject('xml_puny_dom', version: '>=2.2.1')
xml_puny_dom_dependency = xml_puny_dom_subproject.get_variable('xml_puny_dom_library_dependency')

xml_stream_subproject = subproject('xml_stream', version: '>=2.0.0')
xml_stream_dependency = xml_stream_subproject.get_variable('xml_stream_library_dependency')

opengl_dependency = dependency('gl')
x11_dependency = dependency('x11')

subdir('tools')
subdir('data')
subdir('libraries')
subdir('source')
