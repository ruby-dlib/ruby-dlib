#include <dlib/dnn.h>

using namespace std;

static VALUE cDlibCuda;

static VALUE eDlibCudaNotAvailableError;

#define DLIB_RB_ASSERT_CUDA_AVILABILITY()				\
  do {									\
    VALUE ret = dlib_rb_cuda_check_availability();				\
    if (Qtrue != ret) {							\
      rb_raise(eDlibCudaNotAvailableError, "Can't use Cuda Device. Check your installation of CUDA SDK or GPU device"); \
    }									\
  } while(0);


extern "C" VALUE
dlib_rb_cuda_check_availability()
{
#ifdef DLIB_USE_CUDA
  return Qtrue;
#else
  return Qfalse;
#endif
}

extern "C" VALUE
dlib_rb_cuda_set_device(VALUE cuda, VALUE device)
{
  DLIB_RB_ASSERT_CUDA_AVILABILITY();

  int device_id = NUM2INT(device);
  dlib::cuda::set_device(device_id);
  return Qnil;
}

extern "C" VALUE
dlib_rb_cuda_get_device(VALUE cuda)
{
  DLIB_RB_ASSERT_CUDA_AVILABILITY();

  return INT2NUM(dlib::cuda::get_device());
}

extern "C" VALUE
dlib_rb_cuda_get_device_name(VALUE cuda, VALUE device)
{
  DLIB_RB_ASSERT_CUDA_AVILABILITY();

  int device_id = NUM2INT(device);
  std::string string = dlib::cuda::get_device_name(device_id);
  return rb_str_new_cstr(string.c_str());
}

extern "C" VALUE
dlib_rb_cuda_get_num_devices(VALUE cuda)
{
  DLIB_RB_ASSERT_CUDA_AVILABILITY();

  return INT2NUM(dlib::cuda::get_num_devices());
}

static void
Init_dlib_cuda()
{
  cDlibCuda = rb_define_module_under(mDlib, "Cuda");

  eDlibCudaNotAvailableError = rb_define_class_under(cDlibCuda, "CudaNotAvailableError", eDlibError);

  rb_define_singleton_method(cDlibCuda, "available?", RUBY_METHOD_FUNC(dlib_rb_cuda_check_availability), 0);

  rb_define_singleton_method(cDlibCuda, "set_device", RUBY_METHOD_FUNC(dlib_rb_cuda_set_device), 1);

  rb_define_singleton_method(cDlibCuda, "get_device", RUBY_METHOD_FUNC(dlib_rb_cuda_get_device), 0);

  rb_define_singleton_method(cDlibCuda, "get_device_name", RUBY_METHOD_FUNC(dlib_rb_cuda_get_device_name), 1);

  rb_define_singleton_method(cDlibCuda, "get_num_devices", RUBY_METHOD_FUNC(dlib_rb_cuda_get_num_devices), 0);
}
