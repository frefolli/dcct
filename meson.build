project('dcct', 'cpp',
  version : '0.1',
  default_options : ['warning_level=3',
                     'cpp_std=c++20'])

eigen3 = dependency('eigen3')
fftw3 = dependency('fftw3')

dcct = library('dcct', [
    './src/dcct/logging.cc',
    './src/dcct/matrix.cc',
    './src/dcct/specifiers.cc',
    './src/dcct/testing.cc',
    './src/dcct/slow_actuator.cc',
    './src/dcct/fast_actuator.cc',
    './src/dcct/fftw_actuator.cc',
  ], install: true,
  include_directories: ['./include'],
  dependencies: [eigen3, fftw3])

exe = executable('main.exe', [
    'src/main.cc',
  ], install : false,
  include_directories: './include',
  link_with: dcct)