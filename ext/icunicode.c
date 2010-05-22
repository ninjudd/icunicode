#include "ruby.h"
#include "unicode/ucol.h"
#include "unicode/utrans.h"
#include "unicode/ustring.h"
#include "unicode/ustdio.h"

#define BUF_SIZE 1000

VALUE cTransliterator;
VALUE trans_hash;

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

static void trans_free(void *trans) {
  utrans_close(trans);
}

static UTransliterator* get_trans(VALUE transform) {
  UChar str[BUF_SIZE];
  int32_t len = 0;
  UTransliterator *trans;
  UErrorCode status = U_ZERO_ERROR;
  VALUE obj;

  obj = rb_hash_aref(trans_hash, transform);
  if (NIL_P(obj)) {
    to_utf16(transform, str, &len);
    trans = utrans_openU(str, len, UTRANS_FORWARD, NULL, 0, NULL, &status);
    if (trans) {
      obj = Data_Wrap_Struct(rb_cObject, 0, trans_free, trans);
      rb_hash_aset(trans_hash, transform, obj);
    } else {
      rb_raise(rb_eArgError, "invalid transform: %s", RSTRING_PTR(transform));
    }
  }

  Data_Get_Struct(obj, UTransliterator, trans);
  return trans;
}

/*
 * call-seq:
 * string.transliterate(transform_string) -> string
 *
 * Transliterates string using transform.
 *
 */
static VALUE unicode_transliterate(int argc, VALUE *argv, VALUE string) {
  UChar str[BUF_SIZE];
  int32_t slen = 0;
  UErrorCode status = U_ZERO_ERROR;
  UTransliterator *trans;
  VALUE transform;

  rb_scan_args(argc, argv, "01", &transform);
  if (NIL_P(transform)) transform = rb_str_new2("Latin; Lower; NFD; [^[:letter:] [:space:] [0-9] [:punctuation:]] Remove; NFC");

  to_utf16(string, str, &slen);

  trans = get_trans(transform);
  utrans_transUChars(trans, str, &slen, BUF_SIZE, 0, &slen, &status);

  to_utf8(str, slen);
}

void Init_icunicode() {
  rb_define_method(rb_cString, "unicode_sort_key", unicode_sort_key, 0);
  rb_define_method(rb_cString, "transliterate", unicode_transliterate, -1);

  trans_hash = rb_hash_new();
  rb_global_variable(&trans_hash);
}
