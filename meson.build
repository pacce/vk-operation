project(
  'vk-operation'
  , 'cpp'
  , version : '0.1.0'
  , default_options : ['warning_level=3', 'cpp_std=c++14']
  )

vulkan        = dependency('vulkan')
dependencies  = [vulkan]

headers = include_directories('include', 'source')
sources = [
    'source/vk-device.cpp'
  , 'source/vk-instance.cpp'
  , 'source/vk-shader.cpp'
  ]

glsl    = find_program('glslangValidator')
shaders = []
foreach s : ['shader/naive-prefix.comp']
  shaders += custom_target(
    'shader @0@'.format(s)
    , command           : [glsl, '-V', '@INPUT@', '-o', '@OUTPUT@']
    , input             : s
    , output            : '@PLAINNAME@.spv'
    , build_by_default  : true
    )
endforeach

executable(
  'vk-prefix'
  , ['source/main-prefix.cpp', sources]
  , include_directories : headers
  , dependencies        : dependencies
  , install             : true
  )
