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

subproject('algebra')
algebra_dependency = dependency(
  'algebra',
  version: '>=2.1.1'
)

expressions_subproject = subproject('expressions')
expressions_dependency = expressions_subproject.get_variable('expressions_library_dependency')

subproject('string_cast')
string_cast_dependency = dependency(
  'string_cast',
  version: '>=2.0.2'
)

subproject('xml_parser')
xml_parser_dependency = dependency(
  'xml_parser',
  version: '>=2.0.2'
)

subproject('xml_puny_dom')
xml_puny_dom_dependency = dependency(
  'xml_puny_dom',
  version: '>=2.2.1'
)

xml_stream_subproject = subproject('xml_stream', version: '>=2.0.0')
xml_stream_dependency = xml_stream_subproject.get_variable('xml_stream_library_dependency')
