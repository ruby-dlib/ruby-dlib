// vim: ft=cpp

#include <iostream>
#include <dlib/dnn.h>
#include <dlib/data_io.h>
#include <dlib/image_processing.h>

using namespace std;
using namespace dlib;


template <long num_filters, typename SUBNET> using con5d = con<num_filters,5,5,2,2,SUBNET>;
template <long num_filters, typename SUBNET> using con5  = con<num_filters,5,5,1,1,SUBNET>;

template <typename SUBNET> using downsampler  = relu<affine<con5d<32, relu<affine<con5d<32, relu<affine<con5d<16,SUBNET>>>>>>>>>;
template <typename SUBNET> using rcon5  = relu<affine<con5<45,SUBNET>>>;

using net_type = loss_mmod<con<1,9,9,1,1,rcon5<rcon5<rcon5<downsampler<input_rgb_image_pyramid<pyramid_down<6>>>>>>>>;

static VALUE cDlibDNNFaceDetector;

struct dnn_fd_container {
  net_type net;
};

static void
dnn_fd_container_free(void *ptr)
{
  dnn_fd_container *dnn_fdcont = static_cast<dnn_fd_container *>(ptr);
  delete dnn_fdcont;
}

static size_t
dnn_fd_container_memsize(void const *ptr)
{
  const dnn_fd_container *dnn_fdcont = static_cast<const dnn_fd_container *>(ptr);
  return sizeof(*dnn_fdcont);
}

static const rb_data_type_t dnn_fd_container_data_type = {
  "DlibDNNFaceDetector",
  {
    0, dnn_fd_container_free, dnn_fd_container_memsize,
  },
#ifdef RUBY_TYPED_FREE_IMMEDIATELY
  0, 0, RUBY_TYPED_FREE_IMMEDIATELY
#endif
};

static dnn_fd_container *
dlib_rb_dnn_fd_get_dnn_fd_container(VALUE dnn_fd)
{
  dnn_fd_container *dnn_fdcont;

  TypedData_Get_Struct(dnn_fd, dnn_fd_container, &dnn_fd_container_data_type, dnn_fdcont);

  return dnn_fdcont;
}

static VALUE
dlib_rb_dnn_fd_alloc(VALUE klass)
{
  dnn_fd_container *dnn_fdcont = new dnn_fd_container;
  VALUE dnn_fd = TypedData_Wrap_Struct(klass, &dnn_fd_container_data_type, dnn_fdcont);
  return dnn_fd;
}

static VALUE
dlib_rb_dnn_fd_initialize(VALUE dnn_fd, VALUE filename)
{
  dnn_fd_container *dnn_fdcont = dlib_rb_dnn_fd_get_dnn_fd_container(dnn_fd);
  dlib::deserialize(StringValueCStr(filename))  >> dnn_fdcont->net;
  return dnn_fd;
}

extern "C" VALUE
dlib_rb_dnn_fd_detect(VALUE dnn_fd, VALUE matrix)
{
  dnn_fd_container *dnn_fdcont = dlib_rb_dnn_fd_get_dnn_fd_container(dnn_fd);
  rgb_matrix_container *matrix_container = dlib_rb_matrix_get_rgb_matrix_container(matrix);
  auto detections = dnn_fdcont->net(matrix_container->matrix);

  VALUE result = rb_ary_new_capa(detections.size());
  for (auto&& detection : detections) {
    rb_ary_push(result, dlib_rb_rectangle_new(detection.rect));
  }
  return result;
}

static void
Init_dlib_dnn_detector()
{
  cDlibDNNFaceDetector = rb_define_class_under(mDlib, "DNNFaceDetector", rb_cData);

  rb_define_alloc_func(cDlibDNNFaceDetector, dlib_rb_dnn_fd_alloc);

  rb_define_method(cDlibDNNFaceDetector, "initialize", RUBY_METHOD_FUNC(dlib_rb_dnn_fd_initialize), 1);

  rb_define_method(cDlibDNNFaceDetector, "detect", RUBY_METHOD_FUNC(dlib_rb_dnn_fd_detect), 1);
}
