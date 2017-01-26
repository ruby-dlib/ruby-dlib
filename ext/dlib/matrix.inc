// vim: ft=cpp

#include <dlib/matrix.h>

static VALUE cDlibMatrix;

static VALUE eDlibMatrixLoadError;

struct rgb_matrix_container {
  typedef dlib::matrix<dlib::rgb_pixel> matrix_type;

  matrix_type matrix;
};

static void
rgb_matrix_container_free(void *ptr)
{
  rgb_matrix_container *matrix_container = static_cast<rgb_matrix_container *>(ptr);
  delete matrix_container;
}

static size_t
rgb_matrix_container_memsize(void const *ptr)
{
  rgb_matrix_container const *matrix_container = static_cast<rgb_matrix_container const *>(ptr);
  return sizeof(*matrix_container);
}

static const rb_data_type_t rgb_matrix_container_data_type = {
  "DlibMatrix",
  {
    0, rgb_matrix_container_free, rgb_matrix_container_memsize,
  },
#ifdef RUBY_TYPED_FREE_IMMEDIATELY
  0, 0, RUBY_TYPED_FREE_IMMEDIATELY
#endif
};

static rgb_matrix_container *
dlib_rb_matrix_get_rgb_matrix_container(VALUE matrix)
{
  rgb_matrix_container *matrix_container;

  TypedData_Get_Struct(matrix, rgb_matrix_container, &rgb_matrix_container_data_type, matrix_container);

  return matrix_container;
}

static VALUE
dlib_rb_matrix_alloc(VALUE klass)
{
  rgb_matrix_container *matrix_container = new rgb_matrix_container;
  VALUE matrix = TypedData_Wrap_Struct(klass, &rgb_matrix_container_data_type, matrix_container);
  return matrix;
}

extern "C" VALUE
dlib_rb_matrix_s_load(VALUE klass, VALUE filename)
{
  VALUE matrix = dlib_rb_matrix_alloc(klass);
  rgb_matrix_container *matrix_container = dlib_rb_matrix_get_rgb_matrix_container(matrix);

  try {
    dlib::load_image(matrix_container->matrix, StringValueCStr(filename));
  }
  catch (dlib::image_load_error error) {
    rb_raise(eDlibMatrixLoadError, "%s", error.what());
  }

  return matrix;
}

static void
Init_dlib_matrix()
{
  cDlibMatrix = rb_define_class_under(mDlib, "Matrix", rb_cData);

  eDlibMatrixLoadError = rb_define_class_under(cDlibMatrix, "LoadError", eDlibError);

  rb_define_alloc_func(cDlibMatrix, dlib_rb_matrix_alloc);
  rb_define_singleton_method(cDlibMatrix, "load", RUBY_METHOD_FUNC(dlib_rb_matrix_s_load), 1);
}
