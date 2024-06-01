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

test_slow_dct = executable('test_slow_dct', [
    'tests/src/dcct/slow_dct.cc',
  ], install : false,
  include_directories: ['./include', './tests/include'],
  link_with: dcct)
test('Slow DCT Test', test_slow_dct)

test_slow_dct2 = executable('test_slow_dct2', [
    'tests/src/dcct/slow_dct2.cc',
  ], install : false,
  include_directories: ['./include', './tests/include'],
  link_with: dcct)
test('Slow DCT2 Test', test_slow_dct2)

test_fast_dct = executable('test_fast_dct', [
    'tests/src/dcct/fast_dct.cc',
  ], install : false,
  include_directories: ['./include', './tests/include'],
  link_with: dcct)
test('Fast DCT Test', test_fast_dct)

test_fast_dct2 = executable('test_fast_dct2', [
    'tests/src/dcct/fast_dct2.cc',
  ], install : false,
  include_directories: ['./include', './tests/include'],
  link_with: dcct)
test('Fast DCT2 Test', test_fast_dct2)

test_fftw_dct = executable('test_fftw_dct', [
    'tests/src/dcct/fftw_dct.cc',
  ], install : false,
  include_directories: ['./include', './tests/include'],
  link_with: dcct)
test('FFTW DCT Test', test_fftw_dct)

test_fftw_dct2 = executable('test_fftw_dct2', [
    'tests/src/dcct/fftw_dct2.cc',
  ], install : false,
  include_directories: ['./include', './tests/include'],
  link_with: dcct)
test('FFTW DCT2 Test', test_fftw_dct2)

test_slow_fast_dct = executable('test_slow_fast_dct', [
    'tests/src/dcct/slow_fast_dct.cc',
  ], install : false,
  include_directories: ['./include', './tests/include'],
  link_with: dcct)
test('Slow-Fast DCT Consistency Test', test_slow_fast_dct)

test_fftw_fast_dct = executable('test_fftw_fast_dct', [
    'tests/src/dcct/fftw_fast_dct.cc',
  ], install : false,
  include_directories: ['./include', './tests/include'],
  link_with: dcct)
test('FFTW-Fast DCT Consistency Test', test_fftw_fast_dct)

test_slow_fast_dct2 = executable('test_slow_fast_dct2', [
    'tests/src/dcct/slow_fast_dct2.cc',
  ], install : false,
  include_directories: ['./include', './tests/include'],
  link_with: dcct)
test('Slow-Fast DCT2 Consistency Test', test_slow_fast_dct2)

test_fftw_fast_dct2 = executable('test_fftw_fast_dct2', [
    'tests/src/dcct/fftw_fast_dct2.cc',
  ], install : false,
  include_directories: ['./include', './tests/include'],
  link_with: dcct)
test('FFTW-Fast DCT2 Consistency Test', test_fftw_fast_dct2)

test_slow_idct = executable('test_slow_idct', [
    'tests/src/dcct/slow_idct.cc',
  ], install : false,
  include_directories: ['./include', './tests/include'],
  link_with: dcct)
test('Slow IDCT Test', test_slow_idct)

test_slow_idct2 = executable('test_slow_idct2', [
    'tests/src/dcct/slow_idct2.cc',
  ], install : false,
  include_directories: ['./include', './tests/include'],
  link_with: dcct)
test('Slow IDCT2 Test', test_slow_idct2)

test_fast_idct = executable('test_fast_idct', [
    'tests/src/dcct/fast_idct.cc',
  ], install : false,
  include_directories: ['./include', './tests/include'],
  link_with: dcct)
test('Fast IDCT Test', test_fast_idct)

test_fast_idct2 = executable('test_fast_idct2', [
    'tests/src/dcct/fast_idct2.cc',
  ], install : false,
  include_directories: ['./include', './tests/include'],
  link_with: dcct)
test('Fast IDCT2 Test', test_fast_idct2)

test_fftw_idct = executable('test_fftw_idct', [
    'tests/src/dcct/fftw_idct.cc',
  ], install : false,
  include_directories: ['./include', './tests/include'],
  link_with: dcct)
test('FFTW IDCT Test', test_fftw_idct)

test_fftw_idct2 = executable('test_fftw_idct2', [
    'tests/src/dcct/fftw_idct2.cc',
  ], install : false,
  include_directories: ['./include', './tests/include'],
  link_with: dcct)
test('FFTW IDCT2 Test', test_fftw_idct2)

test_slow_fast_idct = executable('test_slow_fast_idct', [
    'tests/src/dcct/slow_fast_idct.cc',
  ], install : false,
  include_directories: ['./include', './tests/include'],
  link_with: dcct)
test('Slow-Fast IDCT Consistency Test', test_slow_fast_idct)

test_fftw_fast_idct = executable('test_fftw_fast_idct', [
    'tests/src/dcct/fftw_fast_idct.cc',
  ], install : false,
  include_directories: ['./include', './tests/include'],
  link_with: dcct)
test('FFTW-Fast IDCT Consistency Test', test_fftw_fast_idct)

test_slow_fast_idct2 = executable('test_slow_fast_idct2', [
    'tests/src/dcct/slow_fast_idct2.cc',
  ], install : false,
  include_directories: ['./include', './tests/include'],
  link_with: dcct)
test('Slow-Fast IDCT2 Consistency Test', test_slow_fast_idct2)

test_fftw_fast_idct2 = executable('test_fftw_fast_idct2', [
    'tests/src/dcct/fftw_fast_idct2.cc',
  ], install : false,
  include_directories: ['./include', './tests/include'],
  link_with: dcct)
test('FFTW-Fast IDCT2 Consistency Test', test_fftw_fast_idct2)

exe = executable('main.exe', [
    'src/main.cc',
  ], install : false,
  include_directories: './include',
  link_with: dcct)