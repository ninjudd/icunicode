#include "ruby.h"
#include "unicode/ucol.h"
#include "unicode/utrans.h"
#include "unicode/ustring.h"
#include "unicode/ustdio.h"
 
#define BUF_SIZE 1000

static void to_utf16(VALUE string, UChar *ustr, int32_t *ulen) {
  UErrorCode status = U_ZERO_ERROR; 

  string = StringValue(string);
  u_strFromUTF8(ustr, BUF_SIZE, ulen, RSTRING_PTR(string), RSTRING_LEN(string), &status);
  if (status == U_INVALID_CHAR_FOUND) ulen = 0;
}

static VALUE to_utf8(UChar *ustr, int32_t ulen) {
  char str[BUF_SIZE];
  int32_t len = 0;
  UErrorCode status = U_ZERO_ERROR; 

  u_strToUTF8(str, BUF_SIZE, &len, ustr, ulen, &status);
  if (status == U_INVALID_CHAR_FOUND) len = 0;
  return rb_str_new(str, len);
}

/*
 * call-seq:
 * string.unicode_sort_key -> string
 *
 * Returns a string that will sort according to the Unicode collation algorithm.
 *
 */
static VALUE unicode_sort_key(VALUE string) {
  char str[BUF_SIZE];
  UChar ustr[BUF_SIZE];
  int32_t len  = 0;
  int32_t ulen = 0;
  UErrorCode status = U_ZERO_ERROR; 
  UCollator *col;

  to_utf16(string, ustr, &ulen);

  col = ucol_open("en_US", &status); 
  if (U_SUCCESS(status)) {
    len = ucol_getSortKey(col, ustr, ulen, (uint8_t*)str, BUF_SIZE);
    ucol_close(col);
  }    
  
  return rb_str_new(str, len - 1);
}

/*
 * call-seq:
 * string.transliterate(transform) -> string
 *
 * Transliterates string using transform.
 *
 */
static VALUE unicode_transliterate(VALUE string, VALUE transform) {
  UChar str[BUF_SIZE];
  UChar trn[BUF_SIZE];
  int32_t slen = 0;
  int32_t tlen = 0;
  UErrorCode  status = U_ZERO_ERROR; 
  UTransliterator *trans;

  to_utf16(string,    str, &slen);
  to_utf16(transform, trn, &tlen);

  trans = utrans_openU(trn, tlen, UTRANS_FORWARD, NULL, 0, NULL, &status);
  if (trans) {
    utrans_transUChars(trans, str, &slen, BUF_SIZE, 0, &slen, &status);
    utrans_close(trans);
  } else {
    rb_raise(rb_eArgError, "invalid transform: %s", RSTRING_PTR(transform));
  }

  to_utf8(str, slen);
}
 
void Init_icunicode() {
  rb_define_method(rb_cString, "unicode_sort_key", unicode_sort_key, 0);
  rb_define_method(rb_cString, "transliterate", unicode_transliterate, 1);
}
