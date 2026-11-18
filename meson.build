project(
  'galactic-fall',
  'cpp',
  version: '0.1',
  meson_version: '>=1.8.2',
  default_options: {
    'cpp_std': 'c++26',
    'default_library': 'static',
    'warning_level': '3',
  },
)

python_module = import('python')
python3_program = python_module.find_installation('python3')

subproject('arx')
arx_dependency = dependency('arx', version: '>=26.11.0')

subproject(
  'algebra',
  default_options: {
    'tests': 'enabled',
  },
)
algebra_dependency = dependency('algebra', version: '>=26.11.1')

# dependency "expressions"
subproject(
  'expressions',
  default_options: {
    'tests': 'enabled',
    'trace_logging': 'disabled',
  }
)
expressions_dependency = dependency('expressions', version: '>=26.11.1')

subproject('xml_parser')
xml_parser_dependency = dependency('xml_parser', version: '>=26.10.2')

subproject(
  'rasteriser',
  default_options: {
    'tests': 'enabled',
  },
)
rasteriser_dependency = dependency('rasteriser', version: '>=26.10.0')

subproject(
  'signals',
  default_options: {
    'tests': 'enabled',
  }
)
signals_dependency = dependency('signals', version: '>=26.11.0')

string_cast_subproject = subproject('string_cast', version: '>=2.0.2')
string_cast_dependency = string_cast_subproject.get_variable('string_cast_library_dependency')

subproject('xml_puny_dom')
xml_puny_dom_dependency = dependency('xml_puny_dom', version: '>=26.10.1')

xml_stream_subproject = subproject('xml_stream', version: '>=2.0.0')
xml_stream_dependency = xml_stream_subproject.get_variable('xml_stream_library_dependency')

opengl_dependency = dependency('gl')
x11_dependency = dependency('x11')

subdir('tools')
subdir('data')
subdir('libraries')
subdir('source')
