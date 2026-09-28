subdir('tools')

custom_target(
  'data.arx',
  build_by_default: true,
  input: 'data.arx',
  output: 'data.arx',
  command: [
    python3_program,
    arxbuild_py_script,
    '@INPUT@',
    manifest_json,
    '@OUTPUT@'
  ]
)
