arxx_library = static_library(
  'arxx',
  sources: files(
    'source/archive.cpp',
    'source/buffer.cpp',
    'source/buffer_reader.cpp',
    'source/buffer_writer.cpp',
    'source/common.cpp',
    'source/data.cpp',
    'source/item.cpp',
    'source/reference.cpp',
    'source/reference_implementation.cpp',
    'source/relation.cpp',
    'source/structure.cpp'
  ),
  include_directories: [include_directories('include')]
)

arxx_dependency = declare_dependency(
  include_directories: [include_directories('include')],
  link_with: [arxx_library]
)
