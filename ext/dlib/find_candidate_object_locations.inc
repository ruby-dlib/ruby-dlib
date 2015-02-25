// vim: ft=cpp

#define DEFAULT_KVALS                  rb_ary_new_from_args(3, INT2FIX(50), INT2FIX(200), INT2FIX(3))
#define DEFAULT_MIN_SIZE               20
#define DEFAULT_MAX_MERGING_ITERATIONS 50

static VALUE
dlib_rb_find_candidate_object_locations(int argc, VALUE* argv, VALUE mod)
{
  static ID keyword_ids[3];

  VALUE image, opts = Qnil;
  VALUE kvals_v = Qnil, min_size_v = Qnil, max_merging_iterations_v = Qnil;

  double k_start, k_end;
  long k_num;
  unsigned long min_size;
  unsigned long max_merging_iterations;

  rb_scan_args(argc, argv, "1:", &image, &opts);

  if (!NIL_P(opts)) {
    VALUE kwvals[3];

    if (!keyword_ids[0]) {
      keyword_ids[0] = rb_intern("kvals");
      keyword_ids[1] = rb_intern("min_size");
      keyword_ids[2] = rb_intern("max_merging_iterations");
    }

    rb_get_kwargs(opts, keyword_ids, 0, 3, kwvals);

    if (kwvals[0] != Qundef) kvals_v = kwvals[0];
    if (kwvals[1] != Qundef) min_size_v = kwvals[1];
    if (kwvals[2] != Qundef) max_merging_iterations_v = kwvals[2];
  }


  if (NIL_P(kvals_v)) {
    kvals_v = DEFAULT_KVALS;
  }
  else {
    Check_Type(kvals_v, T_ARRAY);
    if (RARRAY_LEN(kvals_v) != 3) {
      rb_raise(rb_eArgError, "kvals must be an array with three elements for start, end, num.");
    }
  }

  k_start = NUM2DBL(RARRAY_AREF(kvals_v, 0));
  k_end   = NUM2DBL(RARRAY_AREF(kvals_v, 1));
  k_num   = NUM2LONG(RARRAY_AREF(kvals_v, 2));


  if (NIL_P(min_size_v)) {
    min_size = DEFAULT_MIN_SIZE;
  }
  else {
    min_size = NUM2ULONG(min_size_v);
  }


  if (NIL_P(max_merging_iterations_v)) {
    max_merging_iterations = DEFAULT_MAX_MERGING_ITERATIONS;
  }
  else {
    max_merging_iterations = NUM2ULONG(max_merging_iterations_v);
  }

  rgb_image_container *image_container = dlib_rb_image_get_rgb_image_container(image);

  dlib::matrix_range_exp<double> kvals = dlib::linspace(k_start, k_end, k_num);
  typedef std::vector<dlib::rectangle> rectangles_type;
  rectangles_type rects(10);
  dlib::find_candidate_object_locations(image_container->image, rects, kvals, min_size, max_merging_iterations);

  VALUE ary = rb_ary_new_capa(rects.size());
  for (rectangles_type::const_iterator i = rects.begin(); i != rects.end(); ++i) {
    VALUE rect = dlib_rb_rectangle_new(*i);
    rb_ary_push(ary, rect);
  }

  return ary;
}
