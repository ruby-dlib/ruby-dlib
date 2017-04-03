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

  try {
    dlib::deserialize(StringValueCStr(filename)) >> dnn_fdcont->net;
  } catch (dlib::cuda_error& error) {
    rb_raise(eDlibCudaError, "cuda error during loading model: %s", error.info.c_str());
  } catch (std::bad_alloc& error) {
    rb_raise(rb_eNoMemError, "bad allocation memory during loading model");
  }

  return dnn_fd;
}

extern "C" VALUE
dlib_rb_dnn_fd_detect_in_batches(VALUE dnn_fd, VALUE images)
{
  dnn_fd_container *dnn_fdcont = dlib_rb_dnn_fd_get_dnn_fd_container(dnn_fd);

  std::vector<dlib::matrix<dlib::rgb_pixel>> matrixes;
  for (long i = 0; i < RARRAY_LEN(images); i++) {
    VALUE image = RARRAY_AREF(images, i);
    rgb_image_container *image_container = dlib_rb_image_get_rgb_image_container(image);

    dlib::matrix<dlib::rgb_pixel> matrix;
    assign_image(matrix, image_container->image);
    matrixes.push_back(matrix);
  }

  std::vector<std::vector<dlib::mmod_rect>> detections_in_batches;
  try {
    detections_in_batches = dnn_fdcont->net(matrixes);
  } catch (dlib::cuda_error& error) {
    dnn_fdcont->net.clean();
    rb_raise(eDlibCudaError, "cuda error during inferencing: %s", error.info.c_str());
  } catch (std::bad_alloc& error) {
    dnn_fdcont->net.clean();
    rb_raise(rb_eNoMemError, "bad allocation memory during inferencing");
  }

  VALUE result_all = rb_ary_new_capa(detections_in_batches.size());
  for (auto&& detections: detections_in_batches) {
    VALUE result = rb_ary_new_capa(detections.size());
    for (auto&& detection : detections) {
      rb_ary_push(result, dlib_rb_rectangle_new(detection.rect));
    }
    rb_ary_push(result_all, result);

    // force free memory on GPU
    detections.clear();
    detections.shrink_to_fit();
  }
  // force free memory on GPU
  detections_in_batches.clear();
  detections_in_batches.shrink_to_fit();

  matrixes.clear();
  matrixes.shrink_to_fit();

  dnn_fdcont->net.clean();

  return result_all;
}

extern "C" VALUE
dlib_rb_dnn_fd_detect_one(VALUE dnn_fd, VALUE image) {
  VALUE images = rb_ary_new_capa(1);
  rb_ary_push(images, image);
  VALUE results = dlib_rb_dnn_fd_detect_in_batches(dnn_fd, images);
  VALUE result = rb_ary_pop(results);
  return result;
}

extern "C" VALUE
dlib_rb_dnn_fd_detect(VALUE dnn_fd, VALUE obj)
{
  VALUE result;
  switch (TYPE(obj)) {
  case T_ARRAY:
    result = dlib_rb_dnn_fd_detect_in_batches(dnn_fd, obj);
    break;
  case T_DATA:
    if (rb_obj_is_instance_of(obj, cDlibImage) != Qtrue) {
      rb_raise(rb_eTypeError, "must give Dlib::Image or array of Dlib::Image");
    }
    result = dlib_rb_dnn_fd_detect_one(dnn_fd, obj);
    break;
  default:
    rb_raise(rb_eTypeError, "must give Dlib::Image or array of Dlib::Image");
    break;
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
