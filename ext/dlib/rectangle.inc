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
dlib_rb_rectangle_new(dlib::rectangle const &dlib_rect)
{
  VALUE rect = dlib_rb_rectangle_alloc(cDlibRectangle);
  rectangle_container *rectcont = dlib_rb_rectangle_get_rectangle_container(rect);
  rectcont->rect = dlib_rect;
  return rect;
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

static void
Init_dlib_rectangle()
{
  cDlibRectangle = rb_define_class_under(mDlib, "Rectangle", rb_cData);

  rb_define_alloc_func(cDlibRectangle, dlib_rb_rectangle_alloc);

  rb_define_method(cDlibRectangle, "left", RUBY_METHOD_FUNC(dlib_rb_rectangle_left), 0);
  rb_define_method(cDlibRectangle, "top", RUBY_METHOD_FUNC(dlib_rb_rectangle_top), 0);
  rb_define_method(cDlibRectangle, "right", RUBY_METHOD_FUNC(dlib_rb_rectangle_right), 0);
  rb_define_method(cDlibRectangle, "bottom", RUBY_METHOD_FUNC(dlib_rb_rectangle_bottom), 0);
}
