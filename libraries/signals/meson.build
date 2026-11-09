signals_library = static_library(
  'signals',
  sources: ['source/connection.cpp'],
  include_directories: [include_directories('include')]
)

signals_dependency = declare_dependency(
  include_directories: [include_directories('include')],
  link_with: [signals_library]
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
