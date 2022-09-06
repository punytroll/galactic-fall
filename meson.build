project(
  'galactic-fall',
  'cpp',
  version: '0.1',
  meson_version: '>=0.63.1',
  default_options: [
    'cpp_std=c++20',
    'default_library=static',
    'werror=true'
  ]
)

subproject('algebra')
algebra_dependency = dependency(
  'algebra',
  version: '>=2.1.1'
)

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
