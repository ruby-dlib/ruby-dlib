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
$CPPFLAGS << " -I#{DLIB_SRCDIR} -std=c++11"
$ARCH_FLAG = '-march=native'

use_cuda = File.exist?('/usr/local/cuda/lib64/libcudart.so')
if use_cuda
  $defs << '-DDLIB_USE_CUDA'
  $CPPFLAGS << " -I/usr/local/cuda/include"
  $LIBS << " -lcudart -lcurand -lcublas -lcudnn"
  $LIBPATH << "/usr/local/cuda/lib64"
end

have_func('rb_get_kwargs')
create_makefile('dlib')
