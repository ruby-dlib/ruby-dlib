#include <ruby.h>
#include <dlib/array2d.h>
#include <dlib/pixel.h>
#include <dlib/image_transforms.h>

#include <iostream>

#include "missing.h"

static VALUE mDlib;

static VALUE eDlibError;

#include "geometry.inc"

#include "image.inc"

#include "detector.inc"

#include "find_candidate_object_locations.inc"

#include "dnn_detector.inc"

#include "cuda.inc"

extern "C" void
Init_dlib(void)
{
  mDlib = rb_define_module("Dlib");

  rb_require("dlib/version");

  eDlibError = rb_define_class_under(mDlib, "Error", rb_eStandardError);

  Init_dlib_geometry();
  Init_dlib_image();
  Init_dlib_detector();
  Init_dlib_dnn_detector();
  Init_dlib_cuda();

  rb_define_module_function(
      mDlib,
      "find_candidate_object_locations",
      RUBY_METHOD_FUNC(dlib_rb_find_candidate_object_locations),
      -1
  );

}
