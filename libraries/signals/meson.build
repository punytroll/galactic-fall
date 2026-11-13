signals_library_include_directories = include_directories('include')

signals_library = static_library(
  'signals',
  sources: [
    'source/connection.cpp',
    'source/scoped_connection.cpp',
  ],
  include_directories: signals_library_include_directories
)

signals_dependency = declare_dependency(
  include_directories: signals_library_include_directories,
  link_with: signals_library
)

signals_library_test_suite_executable = executable(
  'test_suite',
  sources: [
    'testing/test_suite.cpp'
  ],
  dependencies: [
    signals_dependency
  ]
)

test(
  'signals_test_suite',
  signals_library_test_suite_executable
)
