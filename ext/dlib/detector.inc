// vim: ft=cpp

#include <dlib/image_processing/frontal_face_detector.h>

static VALUE cDlibFrontalFaceDetector;

struct ffd_container {
  dlib::frontal_face_detector detector;
};

static void
ffd_container_free(void *ptr)
{
  ffd_container *ffdcont = static_cast<ffd_container *>(ptr);
  delete ffdcont;
}

static size_t
ffd_container_memsize(void const *ptr)
{
  const ffd_container *ffdcont = static_cast<const ffd_container *>(ptr);
  return sizeof(*ffdcont);
}

static const rb_data_type_t ffd_container_data_type = {
  "DlibFrontalFaceDetector",
  {
    0, ffd_container_free, ffd_container_memsize,
  },
#ifdef RUBY_TYPED_FREE_IMMEDIATELY
  0, 0, RUBY_TYPED_FREE_IMMEDIATELY
#endif
};

static ffd_container *
dlib_rb_ffd_get_ffd_container(VALUE ffd)
{
  ffd_container *ffdcont;

  TypedData_Get_Struct(ffd, ffd_container, &ffd_container_data_type, ffdcont);

  return ffdcont;
}

static VALUE
dlib_rb_ffd_alloc(VALUE klass)
{
  ffd_container *ffdcont = new ffd_container;
  VALUE ffd = TypedData_Wrap_Struct(klass, &ffd_container_data_type, ffdcont);
  return ffd;
}

static VALUE
dlib_rb_ffd_initialize(VALUE ffd)
{
  ffd_container *ffdcont = dlib_rb_ffd_get_ffd_container(ffd);
  ffdcont->detector = dlib::get_frontal_face_detector();
  return ffd;
}

extern "C" VALUE
dlib_rb_ffd_detect(VALUE ffd, VALUE image)
{
  ffd_container *ffdcont = dlib_rb_ffd_get_ffd_container(ffd);
  rgb_image_container *image_container = dlib_rb_image_get_rgb_image_container(image);
  std::vector<dlib::rectangle> rects = ffdcont->detector(image_container->image);

  VALUE result = rb_ary_new_capa(rects.size());
  for (std::vector<dlib::rectangle>::iterator it = rects.begin(); it != rects.end(); ++it) {
    rb_ary_push(result, dlib_rb_rectangle_new(*it));
  }
  return result;
}

static void
Init_dlib_detector()
{
  cDlibFrontalFaceDetector = rb_define_class_under(mDlib, "FrontalFaceDetector", rb_cData);

  rb_define_alloc_func(cDlibFrontalFaceDetector, dlib_rb_ffd_alloc);

  rb_define_method(cDlibFrontalFaceDetector, "initialize", RUBY_METHOD_FUNC(dlib_rb_ffd_initialize), 0);

  rb_define_method(cDlibFrontalFaceDetector, "detect", RUBY_METHOD_FUNC(dlib_rb_ffd_detect), 1);
}
