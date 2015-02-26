// vim: ft=cpp

static VALUE cDlibRectangle;

struct rectangle_container {
  dlib::rectangle rect;
};

static void
rectangle_container_free(void *ptr)
{
  rectangle_container *rectcont = static_cast<rectangle_container *>(ptr);
  delete rectcont;
}

static size_t
rectangle_container_memsize(void const *ptr)
{
  rectangle_container const *rectcont = static_cast<rectangle_container const *>(ptr);
  return sizeof(*rectcont);
}

static const rb_data_type_t rectangle_container_data_type = {
  "DlibRectangle",
  {
    0, rectangle_container_free, rectangle_container_memsize,
  },
#ifdef RUBY_TYPED_FREE_IMMEDIATELY
  0, 0, RUBY_TYPED_FREE_IMMEDIATELY
#endif
};

#define is_data_rectangle(obj) (RTYPEDDATA_P((obj)) && RTYPEDDATA_TYPE((obj)) == &rectangle_container_data_type)
#define is_obj_dlib_rectangle(obj) (RB_TYPE_P((obj), T_DATA) && is_data_rectangle(obj))

static rectangle_container *
dlib_rb_rectangle_get_rectangle_container(VALUE rect)
{
  rectangle_container *rectcont;

  TypedData_Get_Struct(rect, rectangle_container, &rectangle_container_data_type, rectcont);

  return rectcont;
}

static VALUE
dlib_rb_rectangle_alloc(VALUE klass)
{
  rectangle_container *rectcont = new rectangle_container;
  VALUE rect = TypedData_Wrap_Struct(klass, &rectangle_container_data_type, rectcont);
  return rect;
}

static VALUE
dlib_rb_rectangle_s_new(VALUE klass, dlib::rectangle const &dlib_rect)
{
  VALUE rect = dlib_rb_rectangle_alloc(klass);
  rectangle_container *rectcont = dlib_rb_rectangle_get_rectangle_container(rect);
  rectcont->rect = dlib_rect;
  return rect;
}

static VALUE
dlib_rb_rectangle_new(dlib::rectangle const &dlib_rect)
{
  return dlib_rb_rectangle_s_new(cDlibRectangle, dlib_rect);
}

static VALUE
dlib_rb_rectangle_initialize(int argc, VALUE* argv, VALUE rect)
{
  rectangle_container *rectcont = dlib_rb_rectangle_get_rectangle_container(rect);

  switch (argc) {
    case 4: {
      long left, top, right, bottom;
      left = NUM2LONG(argv[0]);
      top = NUM2LONG(argv[1]);
      right = NUM2LONG(argv[2]);
      bottom = NUM2LONG(argv[3]);
      rectcont->rect = dlib::rectangle(left, top, right, bottom);
      break;
    }

    case 2: {
      unsigned long width, height;
      width  = NUM2ULONG(argv[0]);
      height = NUM2ULONG(argv[1]);
      rectcont->rect = dlib::rectangle(width, height);
      break;
    }

    case 0:
      // Nothing to do.
      break;

    default:
      rb_raise(rb_eArgError, "Irregal number of arguments are given");
  }

  return rect;
}

extern "C" VALUE
dlib_rb_rectangle_eq(VALUE rect, VALUE other)
{
  rectangle_container *rectcont = dlib_rb_rectangle_get_rectangle_container(rect);
  if (is_obj_dlib_rectangle(other)) {
    rectangle_container *rectcont_other = dlib_rb_rectangle_get_rectangle_container(other);
    return rectcont->rect == rectcont_other->rect ? Qtrue : Qfalse;
  }
  return Qfalse;
}

extern "C" VALUE
dlib_rb_rectangle_left(VALUE rect)
{
  rectangle_container *rectcont = dlib_rb_rectangle_get_rectangle_container(rect);
  return LONG2NUM(rectcont->rect.left());
}

extern "C" VALUE
dlib_rb_rectangle_top(VALUE rect)
{
  rectangle_container *rectcont = dlib_rb_rectangle_get_rectangle_container(rect);
  return LONG2NUM(rectcont->rect.top());
}

extern "C" VALUE
dlib_rb_rectangle_right(VALUE rect)
{
  rectangle_container *rectcont = dlib_rb_rectangle_get_rectangle_container(rect);
  return LONG2NUM(rectcont->rect.right());
}

extern "C" VALUE
dlib_rb_rectangle_bottom(VALUE rect)
{
  rectangle_container *rectcont = dlib_rb_rectangle_get_rectangle_container(rect);
  return LONG2NUM(rectcont->rect.bottom());
}

extern "C" VALUE
dlib_rb_rectangle_width(VALUE rect)
{
  rectangle_container *rectcont = dlib_rb_rectangle_get_rectangle_container(rect);
  return ULONG2NUM(rectcont->rect.width());
}

extern "C" VALUE
dlib_rb_rectangle_height(VALUE rect)
{
  rectangle_container *rectcont = dlib_rb_rectangle_get_rectangle_container(rect);
  return ULONG2NUM(rectcont->rect.height());
}

extern "C" VALUE
dlib_rb_rectangle_area(VALUE rect)
{
  rectangle_container *rectcont = dlib_rb_rectangle_get_rectangle_container(rect);
  return ULONG2NUM(rectcont->rect.area());
}

extern "C" VALUE
dlib_rb_rectangle_is_empty(VALUE rect)
{
  rectangle_container *rectcont = dlib_rb_rectangle_get_rectangle_container(rect);
  return rectcont->rect.is_empty() ? Qtrue : Qfalse;
}

extern "C" VALUE
dlib_rb_rectangle_set_left(VALUE rect, VALUE val)
{
  rectangle_container *rectcont = dlib_rb_rectangle_get_rectangle_container(rect);
  rectcont->rect.set_left(NUM2LONG(val));
  return val;
}

extern "C" VALUE
dlib_rb_rectangle_set_top(VALUE rect, VALUE val)
{
  rectangle_container *rectcont = dlib_rb_rectangle_get_rectangle_container(rect);
  rectcont->rect.set_top(NUM2LONG(val));
  return val;
}

extern "C" VALUE
dlib_rb_rectangle_set_right(VALUE rect, VALUE val)
{
  rectangle_container *rectcont = dlib_rb_rectangle_get_rectangle_container(rect);
  rectcont->rect.set_right(NUM2LONG(val));
  return val;
}

extern "C" VALUE
dlib_rb_rectangle_set_bottom(VALUE rect, VALUE val)
{
  rectangle_container *rectcont = dlib_rb_rectangle_get_rectangle_container(rect);
  rectcont->rect.set_bottom(NUM2LONG(val));
  return val;
}

extern "C" VALUE
dlib_rb_rectangle_union(VALUE rect1, VALUE rect2)
{
  rectangle_container *rectcont1 = dlib_rb_rectangle_get_rectangle_container(rect1);
  rectangle_container *rectcont2 = dlib_rb_rectangle_get_rectangle_container(rect2);
  dlib::rectangle dlib_rect3 = rectcont1->rect + rectcont2->rect;
  return dlib_rb_rectangle_s_new(CLASS_OF(rect1), dlib_rect3);
}

extern "C" VALUE
dlib_rb_rectangle_intersect(VALUE rect1, VALUE rect2)
{
  rectangle_container *rectcont1 = dlib_rb_rectangle_get_rectangle_container(rect1);
  rectangle_container *rectcont2 = dlib_rb_rectangle_get_rectangle_container(rect2);
  dlib::rectangle dlib_rect3 = rectcont1->rect.intersect(rectcont2->rect);
  return dlib_rb_rectangle_s_new(CLASS_OF(rect1), dlib_rect3);
}

extern "C" VALUE
dlib_rb_rectangle_inspect(VALUE rect)
{
  rectangle_container *rectcont = dlib_rb_rectangle_get_rectangle_container(rect);

  VALUE cname = rb_class_name(CLASS_OF(rect));
  VALUE str = rb_sprintf(
    "#<%"PRIsVALUE" (%ld, %ld)-(%ld, %ld) %lux%lu%s>",
    cname,
    rectcont->rect.left(),
    rectcont->rect.top(),
    rectcont->rect.right(),
    rectcont->rect.bottom(),
    rectcont->rect.width(),
    rectcont->rect.height(),
    (rectcont->rect.is_empty() ? " empty" : "")
  );

  return str;
}

static void
Init_dlib_rectangle()
{
  cDlibRectangle = rb_define_class_under(mDlib, "Rectangle", rb_cData);

  rb_define_alloc_func(cDlibRectangle, dlib_rb_rectangle_alloc);

  rb_define_method(cDlibRectangle, "initialize", RUBY_METHOD_FUNC(dlib_rb_rectangle_initialize), -1);

  rb_define_method(cDlibRectangle, "==", RUBY_METHOD_FUNC(dlib_rb_rectangle_eq), 1);

  rb_define_method(cDlibRectangle, "left", RUBY_METHOD_FUNC(dlib_rb_rectangle_left), 0);
  rb_define_method(cDlibRectangle, "top", RUBY_METHOD_FUNC(dlib_rb_rectangle_top), 0);
  rb_define_method(cDlibRectangle, "right", RUBY_METHOD_FUNC(dlib_rb_rectangle_right), 0);
  rb_define_method(cDlibRectangle, "bottom", RUBY_METHOD_FUNC(dlib_rb_rectangle_bottom), 0);
  rb_define_method(cDlibRectangle, "width", RUBY_METHOD_FUNC(dlib_rb_rectangle_width), 0);
  rb_define_method(cDlibRectangle, "height", RUBY_METHOD_FUNC(dlib_rb_rectangle_height), 0);
  rb_define_method(cDlibRectangle, "area", RUBY_METHOD_FUNC(dlib_rb_rectangle_area), 0);
  rb_define_method(cDlibRectangle, "empty?", RUBY_METHOD_FUNC(dlib_rb_rectangle_is_empty), 0);

  rb_define_method(cDlibRectangle, "left=", RUBY_METHOD_FUNC(dlib_rb_rectangle_set_left), 1);
  rb_define_method(cDlibRectangle, "top=", RUBY_METHOD_FUNC(dlib_rb_rectangle_set_top), 1);
  rb_define_method(cDlibRectangle, "right=", RUBY_METHOD_FUNC(dlib_rb_rectangle_set_right), 1);
  rb_define_method(cDlibRectangle, "bottom=", RUBY_METHOD_FUNC(dlib_rb_rectangle_set_bottom), 1);

  rb_define_method(cDlibRectangle, "+", RUBY_METHOD_FUNC(dlib_rb_rectangle_union), 1);
  rb_define_method(cDlibRectangle, "intersect", RUBY_METHOD_FUNC(dlib_rb_rectangle_intersect), 1);

  rb_define_method(cDlibRectangle, "inspect", RUBY_METHOD_FUNC(dlib_rb_rectangle_inspect), 0);
}
