project(
  'UniEngine',
  ['cpp', 'c'],
  version : '0.1',
  meson_version : '>= 1.3.0',
  default_options : ['warning_level=3', 'cpp_std=c++17'],
)

glfw3 = dependency('glfw3', required : true)

dependencies = [
  glfw3
]

inc = include_directories('include')

exe = executable(
  'UniEngine',
  ['uniengine.cpp', 'glad.c'],
  install : true,
  dependencies : dependencies,
  include_directories : [inc]
)

test('basic', exe)
