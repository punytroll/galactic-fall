events_library = static_library(
  'events',
  sources: ['source/connection.cpp'],
  include_directories: [include_directories('include')]
)

events_dependency = declare_dependency(
  include_directories: [include_directories('include')],
  link_with: [events_library]
)

events_library_test_suite_executable = executable(
  'test_suite',
  sources: [
    'testing/test_suite.cpp'
  ],
  dependencies: [
    events_dependency
  ]
)

test(
  'test_suite',
  events_library_test_suite_executable
)
