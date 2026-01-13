project(
  'UniEngine',
  'cpp',
  version : '0.1',
  meson_version : '>= 1.3.0',
  default_options : ['warning_level=3', 'cpp_std=c++14'],
)

dependencies = [
]

inc = include_directories('include')

exe = executable(
  'UniEngine',
  'uniengine.cpp',
  install : true,
  dependencies : dependencies,
  include_directories : [inc]
)

test('basic', exe)
