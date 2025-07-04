arxx_library_cpp_args = []
arxx_library_dependencies = []

zlib_dependency = dependency('zlib', required: get_option('zlib'))
if zlib_dependency.found() == true
  arxx_library_cpp_args += '-DHAVE_ZLIB_H'
  arxx_library_dependencies += zlib_dependency
  message('Arxx library is built with ZLib support.')
elif
  message('Arxx library is built without ZLib support.')
endif
bzip2_dependency = dependency('bzip2', required: get_option('bzip2'))
if bzip2_dependency.found() == true
  arxx_library_cpp_args += '-DHAVE_BZLIB_H'
  arxx_library_dependencies += bzip2_dependency
  message('Arxx library is built with BZip2 support.')
elif
  message('Arxx library is built without BZip2 support.')
endif

arxx_library = static_library(
  'arxx',
  cpp_args: arxx_library_cpp_args,
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
  link_with: [arxx_library],
  dependencies: arxx_library_dependencies
)
