arxx_library = static_library(
  'arxx',
  sources: files(
    'source/Archive.cxx',
    'source/Buffer.cxx',
    'source/BufferReader.cxx',
    'source/BufferWriter.cxx',
    'source/Common.cxx',
    'source/Data.cxx',
    'source/Item.cxx',
    'source/Reference.cxx',
    'source/ReferenceImplementation.cxx',
    'source/Relation.cxx',
    'source/Structure.cxx'
  ),
  include_directories: [include_directories('include')]
)

arxx_library_dependency = declare_dependency(
  include_directories: [include_directories('include')],
  link_with: [arxx_library]
)
