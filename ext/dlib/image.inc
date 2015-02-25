// vim: ft=cpp

#include <dlib/image_io.h>

static VALUE cDlibImage;

static VALUE eDlibImageLoadError;

struct rgb_image_container {
  typedef dlib::array2d<dlib::rgb_pixel> image_type;

  image_type image;
};

static void
rgb_image_container_free(void *ptr)
{
  rgb_image_container *image_container = static_cast<rgb_image_container *>(ptr);
  delete image_container;
}

static size_t
rgb_image_container_memsize(void const *ptr)
{
  rgb_image_container const *image_container = static_cast<rgb_image_container const *>(ptr);
  return sizeof(*image_container);
}

static const rb_data_type_t rgb_image_container_data_type = {
  "DlibImage",
  {
    0, rgb_image_container_free, rgb_image_container_memsize,
  },
#ifdef RUBY_TYPED_FREE_IMMEDIATELY
  0, 0, RUBY_TYPED_FREE_IMMEDIATELY
#endif
};

static rgb_image_container *
dlib_rb_image_get_rgb_image_container(VALUE image)
{
  rgb_image_container *image_container;

  TypedData_Get_Struct(image, rgb_image_container, &rgb_image_container_data_type, image_container);

  return image_container;
}

static VALUE
dlib_rb_image_alloc(VALUE klass)
{
  rgb_image_container *image_container = new rgb_image_container;
  VALUE image = TypedData_Wrap_Struct(klass, &rgb_image_container_data_type, image_container);
  return image;
}

extern "C" VALUE
dlib_rb_image_s_load(VALUE klass, VALUE filename)
{
  VALUE image = dlib_rb_image_alloc(klass);
  rgb_image_container *image_container = dlib_rb_image_get_rgb_image_container(image);

  try {
    dlib::load_image(image_container->image, StringValueCStr(filename));
  }
  catch (dlib::image_load_error error) {
    rb_raise(eDlibImageLoadError, "%s", error.what());
  }

  return image;
}

extern "C" VALUE
dlib_rb_image_nr(VALUE image)
{
  rgb_image_container *image_container = dlib_rb_image_get_rgb_image_container(image);
  long nr = image_container->image.nr();
  return LONG2NUM(nr);
}

extern "C" VALUE
dlib_rb_image_nc(VALUE image)
{
  rgb_image_container *image_container = dlib_rb_image_get_rgb_image_container(image);
  long nc = image_container->image.nc();
  return LONG2NUM(nc);
}

static void
Init_dlib_image()
{
  cDlibImage = rb_define_class_under(mDlib, "Image", rb_cData);

  eDlibImageLoadError = rb_define_class_under(cDlibImage, "LoadError", eDlibError);

  rb_define_alloc_func(cDlibImage, dlib_rb_image_alloc);
  rb_define_singleton_method(cDlibImage, "load", RUBY_METHOD_FUNC(dlib_rb_image_s_load), 1);

  rb_define_method(cDlibImage, "nr", RUBY_METHOD_FUNC(dlib_rb_image_nr), 0);
  rb_define_method(cDlibImage, "nc", RUBY_METHOD_FUNC(dlib_rb_image_nc), 0);
}
