require 'fileutils'
require 'mkmf'

DLIB_VERSION = '19.2'
DLIB_SRCDIR = File.expand_path("../../dlib-#{DLIB_VERSION}", __FILE__)

dir_config('libjpeg')
dir_config('libpng')

have_header('jpeglib.h') or abort
have_library('jpeg', 'jpeg_std_error') or abort

have_header('png.h') or abort
have_library('png', 'png_sig_cmp') or abort

$defs << '-DPNG_SKIP_SETJMP_CHECK'
$defs << '-DDLIB_JPEG_SUPPORT'
$defs << '-DDLIB_PNG_SUPPORT'
$defs << '-DDLIB_NO_GUI_SUPPORT'
$defs << '-DNO_DEBUG'
$defs << '-O3'
$defs << '-march=native'
$CPPFLAGS << " -I#{DLIB_SRCDIR}"

have_func('rb_get_kwargs')

create_makefile('dlib')
