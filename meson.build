project('dcct', 'cpp',
  version : '0.1',
  default_options : ['warning_level=3',
                     'cpp_std=c++20'])

eigen3 = dependency('eigen3')
fftw3 = dependency('fftw3')
jsoncpp = dependency('jsoncpp')
qt6 = import('qt6')
libQt6 = dependency('qt6', modules : 'Widgets')

processed = qt6.preprocess(
  moc_headers : './include/dcct/gui.hh',
)

dcct = library('dcct', [
    './src/dcct/benchmark.cc',
    './src/dcct/fast_actuator.cc',
    './src/dcct/fftw_actuator.cc',
    './src/dcct/logging.cc',
    './src/dcct/matrix.cc',
    './src/dcct/slow_actuator.cc',
    './src/dcct/specifiers.cc',
    './src/dcct/testing.cc',
    './src/dcct/timer.cc',
    './src/dcct/compression.cc',
    './src/dcct/gui.cc',
    processed
  ], install: true,
  include_directories: ['./include'],
  dependencies: [eigen3, fftw3, jsoncpp, libQt6])

subdir('tests')

exe = executable('main.exe', [
    'src/main.cc',
  ], install : false,
  include_directories: './include',
  link_with: dcct)