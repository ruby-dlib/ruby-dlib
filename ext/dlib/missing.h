#ifndef DLIB_MISSING_H
#define DLIB_MISSING_H

#include <ruby/ruby.h>

#if defined(__cplusplus)
extern "C" {
#if 0
} /* satisfy cc-mode */
#endif
#endif


#ifndef HAVE_RB_GET_KWARGS
int rb_get_kwargs(VALUE keyword_hash, const ID *table, int required, int optional, VALUE *values);
#endif


#if defined(__cplusplus)
#if 0
{ /* satisfy cc-mode */
#endif
}  /* extern "C" { */
#endif

#endif /* DLIB_MISSING_H */
