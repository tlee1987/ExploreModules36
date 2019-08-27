
#include <Python.h>
#include <stddef.h>

/* this block of #ifs should be kept exactly identical between
   c/_cffi_backend.c, cffi/vengine_cpy.py, cffi/vengine_gen.py
   and cffi/_cffi_include.h */
#if defined(_MSC_VER)
# include <malloc.h>   /* for alloca() */
# if _MSC_VER < 1600   /* MSVC < 2010 */
   typedef __int8 int8_t;
   typedef __int16 int16_t;
   typedef __int32 int32_t;
   typedef __int64 int64_t;
   typedef unsigned __int8 uint8_t;
   typedef unsigned __int16 uint16_t;
   typedef unsigned __int32 uint32_t;
   typedef unsigned __int64 uint64_t;
   typedef __int8 int_least8_t;
   typedef __int16 int_least16_t;
   typedef __int32 int_least32_t;
   typedef __int64 int_least64_t;
   typedef unsigned __int8 uint_least8_t;
   typedef unsigned __int16 uint_least16_t;
   typedef unsigned __int32 uint_least32_t;
   typedef unsigned __int64 uint_least64_t;
   typedef __int8 int_fast8_t;
   typedef __int16 int_fast16_t;
   typedef __int32 int_fast32_t;
   typedef __int64 int_fast64_t;
   typedef unsigned __int8 uint_fast8_t;
   typedef unsigned __int16 uint_fast16_t;
   typedef unsigned __int32 uint_fast32_t;
   typedef unsigned __int64 uint_fast64_t;
   typedef __int64 intmax_t;
   typedef unsigned __int64 uintmax_t;
# else
#  include <stdint.h>
# endif
# if _MSC_VER < 1800   /* MSVC < 2013 */
#  ifndef __cplusplus
    typedef unsigned char _Bool;
#  endif
# endif
#else
# include <stdint.h>
# if (defined (__SVR4) && defined (__sun)) || defined(_AIX) || defined(__hpux)
#  include <alloca.h>
# endif
#endif

#if PY_MAJOR_VERSION < 3
# undef PyCapsule_CheckExact
# undef PyCapsule_GetPointer
# define PyCapsule_CheckExact(capsule) (PyCObject_Check(capsule))
# define PyCapsule_GetPointer(capsule, name) \
    (PyCObject_AsVoidPtr(capsule))
#endif

#if PY_MAJOR_VERSION >= 3
# define PyInt_FromLong PyLong_FromLong
#endif

#define _cffi_from_c_double PyFloat_FromDouble
#define _cffi_from_c_float PyFloat_FromDouble
#define _cffi_from_c_long PyInt_FromLong
#define _cffi_from_c_ulong PyLong_FromUnsignedLong
#define _cffi_from_c_longlong PyLong_FromLongLong
#define _cffi_from_c_ulonglong PyLong_FromUnsignedLongLong
#define _cffi_from_c__Bool PyBool_FromLong

#define _cffi_to_c_double PyFloat_AsDouble
#define _cffi_to_c_float PyFloat_AsDouble

#define _cffi_from_c_int_const(x)                                        \
    (((x) > 0) ?                                                         \
        ((unsigned long long)(x) <= (unsigned long long)LONG_MAX) ?      \
            PyInt_FromLong((long)(x)) :                                  \
            PyLong_FromUnsignedLongLong((unsigned long long)(x)) :       \
        ((long long)(x) >= (long long)LONG_MIN) ?                        \
            PyInt_FromLong((long)(x)) :                                  \
            PyLong_FromLongLong((long long)(x)))

#define _cffi_from_c_int(x, type)                                        \
    (((type)-1) > 0 ? /* unsigned */                                     \
        (sizeof(type) < sizeof(long) ?                                   \
            PyInt_FromLong((long)x) :                                    \
         sizeof(type) == sizeof(long) ?                                  \
            PyLong_FromUnsignedLong((unsigned long)x) :                  \
            PyLong_FromUnsignedLongLong((unsigned long long)x)) :        \
        (sizeof(type) <= sizeof(long) ?                                  \
            PyInt_FromLong((long)x) :                                    \
            PyLong_FromLongLong((long long)x)))

#define _cffi_to_c_int(o, type)                                          \
    ((type)(                                                             \
     sizeof(type) == 1 ? (((type)-1) > 0 ? (type)_cffi_to_c_u8(o)        \
                                         : (type)_cffi_to_c_i8(o)) :     \
     sizeof(type) == 2 ? (((type)-1) > 0 ? (type)_cffi_to_c_u16(o)       \
                                         : (type)_cffi_to_c_i16(o)) :    \
     sizeof(type) == 4 ? (((type)-1) > 0 ? (type)_cffi_to_c_u32(o)       \
                                         : (type)_cffi_to_c_i32(o)) :    \
     sizeof(type) == 8 ? (((type)-1) > 0 ? (type)_cffi_to_c_u64(o)       \
                                         : (type)_cffi_to_c_i64(o)) :    \
     (Py_FatalError("unsupported size for type " #type), (type)0)))

#define _cffi_to_c_i8                                                    \
                 ((int(*)(PyObject *))_cffi_exports[1])
#define _cffi_to_c_u8                                                    \
                 ((int(*)(PyObject *))_cffi_exports[2])
#define _cffi_to_c_i16                                                   \
                 ((int(*)(PyObject *))_cffi_exports[3])
#define _cffi_to_c_u16                                                   \
                 ((int(*)(PyObject *))_cffi_exports[4])
#define _cffi_to_c_i32                                                   \
                 ((int(*)(PyObject *))_cffi_exports[5])
#define _cffi_to_c_u32                                                   \
                 ((unsigned int(*)(PyObject *))_cffi_exports[6])
#define _cffi_to_c_i64                                                   \
                 ((long long(*)(PyObject *))_cffi_exports[7])
#define _cffi_to_c_u64                                                   \
                 ((unsigned long long(*)(PyObject *))_cffi_exports[8])
#define _cffi_to_c_char                                                  \
                 ((int(*)(PyObject *))_cffi_exports[9])
#define _cffi_from_c_pointer                                             \
    ((PyObject *(*)(char *, CTypeDescrObject *))_cffi_exports[10])
#define _cffi_to_c_pointer                                               \
    ((char *(*)(PyObject *, CTypeDescrObject *))_cffi_exports[11])
#define _cffi_get_struct_layout                                          \
    ((PyObject *(*)(Py_ssize_t[]))_cffi_exports[12])
#define _cffi_restore_errno                                              \
    ((void(*)(void))_cffi_exports[13])
#define _cffi_save_errno                                                 \
    ((void(*)(void))_cffi_exports[14])
#define _cffi_from_c_char                                                \
    ((PyObject *(*)(char))_cffi_exports[15])
#define _cffi_from_c_deref                                               \
    ((PyObject *(*)(char *, CTypeDescrObject *))_cffi_exports[16])
#define _cffi_to_c                                                       \
    ((int(*)(char *, CTypeDescrObject *, PyObject *))_cffi_exports[17])
#define _cffi_from_c_struct                                              \
    ((PyObject *(*)(char *, CTypeDescrObject *))_cffi_exports[18])
#define _cffi_to_c_wchar_t                                               \
    ((wchar_t(*)(PyObject *))_cffi_exports[19])
#define _cffi_from_c_wchar_t                                             \
    ((PyObject *(*)(wchar_t))_cffi_exports[20])
#define _cffi_to_c_long_double                                           \
    ((long double(*)(PyObject *))_cffi_exports[21])
#define _cffi_to_c__Bool                                                 \
    ((_Bool(*)(PyObject *))_cffi_exports[22])
#define _cffi_prepare_pointer_call_argument                              \
    ((Py_ssize_t(*)(CTypeDescrObject *, PyObject *, char **))_cffi_exports[23])
#define _cffi_convert_array_from_object                                  \
    ((int(*)(char *, CTypeDescrObject *, PyObject *))_cffi_exports[24])
#define _CFFI_NUM_EXPORTS 25

typedef struct _ctypedescr CTypeDescrObject;

static void *_cffi_exports[_CFFI_NUM_EXPORTS];
static PyObject *_cffi_types, *_cffi_VerificationError;

static int _cffi_setup_custom(PyObject *lib);   /* forward */

static PyObject *_cffi_setup(PyObject *self, PyObject *args)
{
    PyObject *library;
    int was_alive = (_cffi_types != NULL);
    (void)self; /* unused */
    if (!PyArg_ParseTuple(args, "OOO", &_cffi_types, &_cffi_VerificationError,
                                       &library))
        return NULL;
    Py_INCREF(_cffi_types);
    Py_INCREF(_cffi_VerificationError);
    if (_cffi_setup_custom(library) < 0)
        return NULL;
    return PyBool_FromLong(was_alive);
}

static int _cffi_init(void)
{
    PyObject *module, *c_api_object = NULL;

    module = PyImport_ImportModule("_cffi_backend");
    if (module == NULL)
        goto failure;

    c_api_object = PyObject_GetAttrString(module, "_C_API");
    if (c_api_object == NULL)
        goto failure;
    if (!PyCapsule_CheckExact(c_api_object)) {
        PyErr_SetNone(PyExc_ImportError);
        goto failure;
    }
    memcpy(_cffi_exports, PyCapsule_GetPointer(c_api_object, "cffi"),
           _CFFI_NUM_EXPORTS * sizeof(void *));

    Py_DECREF(module);
    Py_DECREF(c_api_object);
    return 0;

  failure:
    Py_XDECREF(module);
    Py_XDECREF(c_api_object);
    return -1;
}

#define _cffi_type(num) ((CTypeDescrObject *)PyList_GET_ITEM(_cffi_types, num))

/**********/



    #include <sys/stat.h>
    #include "lmdb.h"
    #include "preload.h"

    // Helpers below inline MDB_vals. Avoids key alloc/dup on CPython, where
    // CFFI will use PyString_AS_STRING when passed as an argument.
    static int pymdb_get(MDB_txn *txn, MDB_dbi dbi, char *key_s, size_t keylen,
                         MDB_val *val_out)
    {
        MDB_val key = {keylen, key_s};
        int rc = mdb_get(txn, dbi, &key, val_out);
        preload(rc, val_out->mv_data, val_out->mv_size);
        return rc;
    }

    static int pymdb_put(MDB_txn *txn, MDB_dbi dbi, char *key_s, size_t keylen,
                         char *val_s, size_t vallen, unsigned int flags)
    {
        MDB_val key = {keylen, key_s};
        MDB_val val = {vallen, val_s};
        return mdb_put(txn, dbi, &key, &val, flags);
    }

    static int pymdb_del(MDB_txn *txn, MDB_dbi dbi, char *key_s, size_t keylen,
                         char *val_s, size_t vallen)
    {
        MDB_val key = {keylen, key_s};
        MDB_val val = {vallen, val_s};
        MDB_val *valptr;
        if(vallen == 0) {
            valptr = NULL;
        } else {
            valptr = &val;
        }
        return mdb_del(txn, dbi, &key, valptr);
    }

    static int pymdb_cursor_get(MDB_cursor *cursor,
                                char *key_s, size_t key_len,
                                char *data_s, size_t data_len,
                                MDB_val *key, MDB_val *data, int op)
    {
        MDB_val tmp_key = {key_len, key_s};
        MDB_val tmp_data = {data_len, data_s};
        int rc = mdb_cursor_get(cursor, &tmp_key, &tmp_data, op);
        if(! rc) {
            preload(rc, tmp_data.mv_data, tmp_data.mv_size);
            *key = tmp_key;
            *data = tmp_data;
        }
        return rc;
    }

    static int pymdb_cursor_put(MDB_cursor *cursor, char *key_s, size_t keylen,
                                char *val_s, size_t vallen, int flags)
    {
        MDB_val tmpkey = {keylen, key_s};
        MDB_val tmpval = {vallen, val_s};
        return mdb_cursor_put(cursor, &tmpkey, &tmpval, flags);
    }


static int _cffi_const_MDB_FIRST(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(MDB_FIRST);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "MDB_FIRST", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return ((void)lib,0);
}

static int _cffi_const_MDB_FIRST_DUP(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(MDB_FIRST_DUP);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "MDB_FIRST_DUP", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_MDB_FIRST(lib);
}

static int _cffi_const_MDB_GET_BOTH(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(MDB_GET_BOTH);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "MDB_GET_BOTH", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_MDB_FIRST_DUP(lib);
}

static int _cffi_const_MDB_GET_BOTH_RANGE(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(MDB_GET_BOTH_RANGE);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "MDB_GET_BOTH_RANGE", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_MDB_GET_BOTH(lib);
}

static int _cffi_const_MDB_GET_CURRENT(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(MDB_GET_CURRENT);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "MDB_GET_CURRENT", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_MDB_GET_BOTH_RANGE(lib);
}

static int _cffi_const_MDB_GET_MULTIPLE(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(MDB_GET_MULTIPLE);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "MDB_GET_MULTIPLE", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_MDB_GET_CURRENT(lib);
}

static int _cffi_const_MDB_LAST(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(MDB_LAST);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "MDB_LAST", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_MDB_GET_MULTIPLE(lib);
}

static int _cffi_const_MDB_LAST_DUP(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(MDB_LAST_DUP);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "MDB_LAST_DUP", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_MDB_LAST(lib);
}

static int _cffi_const_MDB_NEXT(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(MDB_NEXT);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "MDB_NEXT", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_MDB_LAST_DUP(lib);
}

static int _cffi_const_MDB_NEXT_DUP(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(MDB_NEXT_DUP);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "MDB_NEXT_DUP", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_MDB_NEXT(lib);
}

static int _cffi_const_MDB_NEXT_MULTIPLE(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(MDB_NEXT_MULTIPLE);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "MDB_NEXT_MULTIPLE", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_MDB_NEXT_DUP(lib);
}

static int _cffi_const_MDB_NEXT_NODUP(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(MDB_NEXT_NODUP);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "MDB_NEXT_NODUP", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_MDB_NEXT_MULTIPLE(lib);
}

static int _cffi_const_MDB_PREV(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(MDB_PREV);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "MDB_PREV", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_MDB_NEXT_NODUP(lib);
}

static int _cffi_const_MDB_PREV_DUP(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(MDB_PREV_DUP);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "MDB_PREV_DUP", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_MDB_PREV(lib);
}

static int _cffi_const_MDB_PREV_NODUP(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(MDB_PREV_NODUP);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "MDB_PREV_NODUP", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_MDB_PREV_DUP(lib);
}

static int _cffi_const_MDB_SET(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(MDB_SET);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "MDB_SET", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_MDB_PREV_NODUP(lib);
}

static int _cffi_const_MDB_SET_KEY(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(MDB_SET_KEY);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "MDB_SET_KEY", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_MDB_SET(lib);
}

static int _cffi_const_MDB_SET_RANGE(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(MDB_SET_RANGE);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "MDB_SET_RANGE", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_MDB_SET_KEY(lib);
}

static PyObject *
_cffi_f_mdb_cursor_close(PyObject *self, PyObject *arg0)
{
  MDB_cursor * x0;
  Py_ssize_t datasize;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(0), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = alloca((size_t)datasize);
    memset((void *)x0, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(0), arg0) < 0)
      return NULL;
  }

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { mdb_cursor_close(x0); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  Py_INCREF(Py_None);
  return Py_None;
}

static PyObject *
_cffi_f_mdb_cursor_count(PyObject *self, PyObject *args)
{
  MDB_cursor * x0;
  size_t * x1;
  Py_ssize_t datasize;
  int result;
  PyObject *arg0;
  PyObject *arg1;

  if (!PyArg_ParseTuple(args, "OO:mdb_cursor_count", &arg0, &arg1))
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(0), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = alloca((size_t)datasize);
    memset((void *)x0, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(0), arg0) < 0)
      return NULL;
  }

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(2), arg1, (char **)&x1);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x1 = alloca((size_t)datasize);
    memset((void *)x1, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x1, _cffi_type(2), arg1) < 0)
      return NULL;
  }

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = mdb_cursor_count(x0, x1); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  return _cffi_from_c_int(result, int);
}

static PyObject *
_cffi_f_mdb_cursor_del(PyObject *self, PyObject *args)
{
  MDB_cursor * x0;
  unsigned int x1;
  Py_ssize_t datasize;
  int result;
  PyObject *arg0;
  PyObject *arg1;

  if (!PyArg_ParseTuple(args, "OO:mdb_cursor_del", &arg0, &arg1))
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(0), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = alloca((size_t)datasize);
    memset((void *)x0, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(0), arg0) < 0)
      return NULL;
  }

  x1 = _cffi_to_c_int(arg1, unsigned int);
  if (x1 == (unsigned int)-1 && PyErr_Occurred())
    return NULL;

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = mdb_cursor_del(x0, x1); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  return _cffi_from_c_int(result, int);
}

static PyObject *
_cffi_f_mdb_cursor_get(PyObject *self, PyObject *args)
{
  MDB_cursor * x0;
  MDB_val * x1;
  MDB_val * x2;
  int x3;
  Py_ssize_t datasize;
  int result;
  PyObject *arg0;
  PyObject *arg1;
  PyObject *arg2;
  PyObject *arg3;

  if (!PyArg_ParseTuple(args, "OOOO:mdb_cursor_get", &arg0, &arg1, &arg2, &arg3))
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(0), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = alloca((size_t)datasize);
    memset((void *)x0, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(0), arg0) < 0)
      return NULL;
  }

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(3), arg1, (char **)&x1);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x1 = alloca((size_t)datasize);
    memset((void *)x1, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x1, _cffi_type(3), arg1) < 0)
      return NULL;
  }

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(3), arg2, (char **)&x2);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x2 = alloca((size_t)datasize);
    memset((void *)x2, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x2, _cffi_type(3), arg2) < 0)
      return NULL;
  }

  x3 = _cffi_to_c_int(arg3, int);
  if (x3 == (int)-1 && PyErr_Occurred())
    return NULL;

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = mdb_cursor_get(x0, x1, x2, x3); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  return _cffi_from_c_int(result, int);
}

static PyObject *
_cffi_f_mdb_cursor_open(PyObject *self, PyObject *args)
{
  MDB_txn * x0;
  unsigned int x1;
  MDB_cursor * * x2;
  Py_ssize_t datasize;
  int result;
  PyObject *arg0;
  PyObject *arg1;
  PyObject *arg2;

  if (!PyArg_ParseTuple(args, "OOO:mdb_cursor_open", &arg0, &arg1, &arg2))
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(4), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = alloca((size_t)datasize);
    memset((void *)x0, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(4), arg0) < 0)
      return NULL;
  }

  x1 = _cffi_to_c_int(arg1, unsigned int);
  if (x1 == (unsigned int)-1 && PyErr_Occurred())
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(5), arg2, (char **)&x2);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x2 = alloca((size_t)datasize);
    memset((void *)x2, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x2, _cffi_type(5), arg2) < 0)
      return NULL;
  }

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = mdb_cursor_open(x0, x1, x2); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  return _cffi_from_c_int(result, int);
}

static PyObject *
_cffi_f_mdb_dbi_flags(PyObject *self, PyObject *args)
{
  MDB_txn * x0;
  unsigned int x1;
  unsigned int * x2;
  Py_ssize_t datasize;
  int result;
  PyObject *arg0;
  PyObject *arg1;
  PyObject *arg2;

  if (!PyArg_ParseTuple(args, "OOO:mdb_dbi_flags", &arg0, &arg1, &arg2))
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(4), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = alloca((size_t)datasize);
    memset((void *)x0, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(4), arg0) < 0)
      return NULL;
  }

  x1 = _cffi_to_c_int(arg1, unsigned int);
  if (x1 == (unsigned int)-1 && PyErr_Occurred())
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(6), arg2, (char **)&x2);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x2 = alloca((size_t)datasize);
    memset((void *)x2, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x2, _cffi_type(6), arg2) < 0)
      return NULL;
  }

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = mdb_dbi_flags(x0, x1, x2); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  return _cffi_from_c_int(result, int);
}

static PyObject *
_cffi_f_mdb_dbi_open(PyObject *self, PyObject *args)
{
  MDB_txn * x0;
  char const * x1;
  unsigned int x2;
  unsigned int * x3;
  Py_ssize_t datasize;
  int result;
  PyObject *arg0;
  PyObject *arg1;
  PyObject *arg2;
  PyObject *arg3;

  if (!PyArg_ParseTuple(args, "OOOO:mdb_dbi_open", &arg0, &arg1, &arg2, &arg3))
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(4), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = alloca((size_t)datasize);
    memset((void *)x0, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(4), arg0) < 0)
      return NULL;
  }

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(7), arg1, (char **)&x1);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x1 = alloca((size_t)datasize);
    memset((void *)x1, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x1, _cffi_type(7), arg1) < 0)
      return NULL;
  }

  x2 = _cffi_to_c_int(arg2, unsigned int);
  if (x2 == (unsigned int)-1 && PyErr_Occurred())
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(6), arg3, (char **)&x3);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x3 = alloca((size_t)datasize);
    memset((void *)x3, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x3, _cffi_type(6), arg3) < 0)
      return NULL;
  }

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = mdb_dbi_open(x0, x1, x2, x3); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  return _cffi_from_c_int(result, int);
}

static PyObject *
_cffi_f_mdb_drop(PyObject *self, PyObject *args)
{
  MDB_txn * x0;
  unsigned int x1;
  int x2;
  Py_ssize_t datasize;
  int result;
  PyObject *arg0;
  PyObject *arg1;
  PyObject *arg2;

  if (!PyArg_ParseTuple(args, "OOO:mdb_drop", &arg0, &arg1, &arg2))
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(4), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = alloca((size_t)datasize);
    memset((void *)x0, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(4), arg0) < 0)
      return NULL;
  }

  x1 = _cffi_to_c_int(arg1, unsigned int);
  if (x1 == (unsigned int)-1 && PyErr_Occurred())
    return NULL;

  x2 = _cffi_to_c_int(arg2, int);
  if (x2 == (int)-1 && PyErr_Occurred())
    return NULL;

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = mdb_drop(x0, x1, x2); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  return _cffi_from_c_int(result, int);
}

static PyObject *
_cffi_f_mdb_env_close(PyObject *self, PyObject *arg0)
{
  MDB_env * x0;
  Py_ssize_t datasize;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(8), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = alloca((size_t)datasize);
    memset((void *)x0, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(8), arg0) < 0)
      return NULL;
  }

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { mdb_env_close(x0); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  Py_INCREF(Py_None);
  return Py_None;
}

static PyObject *
_cffi_f_mdb_env_copy2(PyObject *self, PyObject *args)
{
  MDB_env * x0;
  char const * x1;
  int x2;
  Py_ssize_t datasize;
  int result;
  PyObject *arg0;
  PyObject *arg1;
  PyObject *arg2;

  if (!PyArg_ParseTuple(args, "OOO:mdb_env_copy2", &arg0, &arg1, &arg2))
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(8), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = alloca((size_t)datasize);
    memset((void *)x0, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(8), arg0) < 0)
      return NULL;
  }

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(7), arg1, (char **)&x1);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x1 = alloca((size_t)datasize);
    memset((void *)x1, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x1, _cffi_type(7), arg1) < 0)
      return NULL;
  }

  x2 = _cffi_to_c_int(arg2, int);
  if (x2 == (int)-1 && PyErr_Occurred())
    return NULL;

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = mdb_env_copy2(x0, x1, x2); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  return _cffi_from_c_int(result, int);
}

static PyObject *
_cffi_f_mdb_env_copyfd2(PyObject *self, PyObject *args)
{
  MDB_env * x0;
  int x1;
  int x2;
  Py_ssize_t datasize;
  int result;
  PyObject *arg0;
  PyObject *arg1;
  PyObject *arg2;

  if (!PyArg_ParseTuple(args, "OOO:mdb_env_copyfd2", &arg0, &arg1, &arg2))
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(8), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = alloca((size_t)datasize);
    memset((void *)x0, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(8), arg0) < 0)
      return NULL;
  }

  x1 = _cffi_to_c_int(arg1, int);
  if (x1 == (int)-1 && PyErr_Occurred())
    return NULL;

  x2 = _cffi_to_c_int(arg2, int);
  if (x2 == (int)-1 && PyErr_Occurred())
    return NULL;

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = mdb_env_copyfd2(x0, x1, x2); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  return _cffi_from_c_int(result, int);
}

static PyObject *
_cffi_f_mdb_env_create(PyObject *self, PyObject *arg0)
{
  MDB_env * * x0;
  Py_ssize_t datasize;
  int result;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(9), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = alloca((size_t)datasize);
    memset((void *)x0, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(9), arg0) < 0)
      return NULL;
  }

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = mdb_env_create(x0); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  return _cffi_from_c_int(result, int);
}

static PyObject *
_cffi_f_mdb_env_get_flags(PyObject *self, PyObject *args)
{
  MDB_env * x0;
  unsigned int * x1;
  Py_ssize_t datasize;
  int result;
  PyObject *arg0;
  PyObject *arg1;

  if (!PyArg_ParseTuple(args, "OO:mdb_env_get_flags", &arg0, &arg1))
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(8), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = alloca((size_t)datasize);
    memset((void *)x0, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(8), arg0) < 0)
      return NULL;
  }

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(6), arg1, (char **)&x1);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x1 = alloca((size_t)datasize);
    memset((void *)x1, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x1, _cffi_type(6), arg1) < 0)
      return NULL;
  }

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = mdb_env_get_flags(x0, x1); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  return _cffi_from_c_int(result, int);
}

static PyObject *
_cffi_f_mdb_env_get_maxkeysize(PyObject *self, PyObject *arg0)
{
  MDB_env * x0;
  Py_ssize_t datasize;
  int result;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(8), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = alloca((size_t)datasize);
    memset((void *)x0, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(8), arg0) < 0)
      return NULL;
  }

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = mdb_env_get_maxkeysize(x0); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  return _cffi_from_c_int(result, int);
}

static PyObject *
_cffi_f_mdb_env_get_maxreaders(PyObject *self, PyObject *args)
{
  MDB_env * x0;
  unsigned int * x1;
  Py_ssize_t datasize;
  int result;
  PyObject *arg0;
  PyObject *arg1;

  if (!PyArg_ParseTuple(args, "OO:mdb_env_get_maxreaders", &arg0, &arg1))
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(8), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = alloca((size_t)datasize);
    memset((void *)x0, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(8), arg0) < 0)
      return NULL;
  }

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(6), arg1, (char **)&x1);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x1 = alloca((size_t)datasize);
    memset((void *)x1, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x1, _cffi_type(6), arg1) < 0)
      return NULL;
  }

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = mdb_env_get_maxreaders(x0, x1); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  return _cffi_from_c_int(result, int);
}

static PyObject *
_cffi_f_mdb_env_get_path(PyObject *self, PyObject *args)
{
  MDB_env * x0;
  char const * * x1;
  Py_ssize_t datasize;
  int result;
  PyObject *arg0;
  PyObject *arg1;

  if (!PyArg_ParseTuple(args, "OO:mdb_env_get_path", &arg0, &arg1))
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(8), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = alloca((size_t)datasize);
    memset((void *)x0, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(8), arg0) < 0)
      return NULL;
  }

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(10), arg1, (char **)&x1);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x1 = alloca((size_t)datasize);
    memset((void *)x1, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x1, _cffi_type(10), arg1) < 0)
      return NULL;
  }

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = mdb_env_get_path(x0, x1); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  return _cffi_from_c_int(result, int);
}

static PyObject *
_cffi_f_mdb_env_info(PyObject *self, PyObject *args)
{
  MDB_env * x0;
  MDB_envinfo * x1;
  Py_ssize_t datasize;
  int result;
  PyObject *arg0;
  PyObject *arg1;

  if (!PyArg_ParseTuple(args, "OO:mdb_env_info", &arg0, &arg1))
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(8), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = alloca((size_t)datasize);
    memset((void *)x0, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(8), arg0) < 0)
      return NULL;
  }

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(11), arg1, (char **)&x1);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x1 = alloca((size_t)datasize);
    memset((void *)x1, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x1, _cffi_type(11), arg1) < 0)
      return NULL;
  }

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = mdb_env_info(x0, x1); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  return _cffi_from_c_int(result, int);
}

static PyObject *
_cffi_f_mdb_env_open(PyObject *self, PyObject *args)
{
  MDB_env * x0;
  char const * x1;
  unsigned int x2;
  int x3;
  Py_ssize_t datasize;
  int result;
  PyObject *arg0;
  PyObject *arg1;
  PyObject *arg2;
  PyObject *arg3;

  if (!PyArg_ParseTuple(args, "OOOO:mdb_env_open", &arg0, &arg1, &arg2, &arg3))
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(8), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = alloca((size_t)datasize);
    memset((void *)x0, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(8), arg0) < 0)
      return NULL;
  }

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(7), arg1, (char **)&x1);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x1 = alloca((size_t)datasize);
    memset((void *)x1, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x1, _cffi_type(7), arg1) < 0)
      return NULL;
  }

  x2 = _cffi_to_c_int(arg2, unsigned int);
  if (x2 == (unsigned int)-1 && PyErr_Occurred())
    return NULL;

  x3 = _cffi_to_c_int(arg3, int);
  if (x3 == (int)-1 && PyErr_Occurred())
    return NULL;

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = mdb_env_open(x0, x1, x2, x3); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  return _cffi_from_c_int(result, int);
}

static PyObject *
_cffi_f_mdb_env_set_flags(PyObject *self, PyObject *args)
{
  MDB_env * x0;
  unsigned int x1;
  int x2;
  Py_ssize_t datasize;
  int result;
  PyObject *arg0;
  PyObject *arg1;
  PyObject *arg2;

  if (!PyArg_ParseTuple(args, "OOO:mdb_env_set_flags", &arg0, &arg1, &arg2))
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(8), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = alloca((size_t)datasize);
    memset((void *)x0, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(8), arg0) < 0)
      return NULL;
  }

  x1 = _cffi_to_c_int(arg1, unsigned int);
  if (x1 == (unsigned int)-1 && PyErr_Occurred())
    return NULL;

  x2 = _cffi_to_c_int(arg2, int);
  if (x2 == (int)-1 && PyErr_Occurred())
    return NULL;

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = mdb_env_set_flags(x0, x1, x2); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  return _cffi_from_c_int(result, int);
}

static PyObject *
_cffi_f_mdb_env_set_mapsize(PyObject *self, PyObject *args)
{
  MDB_env * x0;
  size_t x1;
  Py_ssize_t datasize;
  int result;
  PyObject *arg0;
  PyObject *arg1;

  if (!PyArg_ParseTuple(args, "OO:mdb_env_set_mapsize", &arg0, &arg1))
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(8), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = alloca((size_t)datasize);
    memset((void *)x0, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(8), arg0) < 0)
      return NULL;
  }

  x1 = _cffi_to_c_int(arg1, size_t);
  if (x1 == (size_t)-1 && PyErr_Occurred())
    return NULL;

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = mdb_env_set_mapsize(x0, x1); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  return _cffi_from_c_int(result, int);
}

static PyObject *
_cffi_f_mdb_env_set_maxdbs(PyObject *self, PyObject *args)
{
  MDB_env * x0;
  unsigned int x1;
  Py_ssize_t datasize;
  int result;
  PyObject *arg0;
  PyObject *arg1;

  if (!PyArg_ParseTuple(args, "OO:mdb_env_set_maxdbs", &arg0, &arg1))
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(8), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = alloca((size_t)datasize);
    memset((void *)x0, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(8), arg0) < 0)
      return NULL;
  }

  x1 = _cffi_to_c_int(arg1, unsigned int);
  if (x1 == (unsigned int)-1 && PyErr_Occurred())
    return NULL;

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = mdb_env_set_maxdbs(x0, x1); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  return _cffi_from_c_int(result, int);
}

static PyObject *
_cffi_f_mdb_env_set_maxreaders(PyObject *self, PyObject *args)
{
  MDB_env * x0;
  unsigned int x1;
  Py_ssize_t datasize;
  int result;
  PyObject *arg0;
  PyObject *arg1;

  if (!PyArg_ParseTuple(args, "OO:mdb_env_set_maxreaders", &arg0, &arg1))
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(8), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = alloca((size_t)datasize);
    memset((void *)x0, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(8), arg0) < 0)
      return NULL;
  }

  x1 = _cffi_to_c_int(arg1, unsigned int);
  if (x1 == (unsigned int)-1 && PyErr_Occurred())
    return NULL;

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = mdb_env_set_maxreaders(x0, x1); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  return _cffi_from_c_int(result, int);
}

static PyObject *
_cffi_f_mdb_env_stat(PyObject *self, PyObject *args)
{
  MDB_env * x0;
  MDB_stat * x1;
  Py_ssize_t datasize;
  int result;
  PyObject *arg0;
  PyObject *arg1;

  if (!PyArg_ParseTuple(args, "OO:mdb_env_stat", &arg0, &arg1))
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(8), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = alloca((size_t)datasize);
    memset((void *)x0, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(8), arg0) < 0)
      return NULL;
  }

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(12), arg1, (char **)&x1);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x1 = alloca((size_t)datasize);
    memset((void *)x1, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x1, _cffi_type(12), arg1) < 0)
      return NULL;
  }

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = mdb_env_stat(x0, x1); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  return _cffi_from_c_int(result, int);
}

static PyObject *
_cffi_f_mdb_env_sync(PyObject *self, PyObject *args)
{
  MDB_env * x0;
  int x1;
  Py_ssize_t datasize;
  int result;
  PyObject *arg0;
  PyObject *arg1;

  if (!PyArg_ParseTuple(args, "OO:mdb_env_sync", &arg0, &arg1))
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(8), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = alloca((size_t)datasize);
    memset((void *)x0, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(8), arg0) < 0)
      return NULL;
  }

  x1 = _cffi_to_c_int(arg1, int);
  if (x1 == (int)-1 && PyErr_Occurred())
    return NULL;

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = mdb_env_sync(x0, x1); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  return _cffi_from_c_int(result, int);
}

static PyObject *
_cffi_f_mdb_get(PyObject *self, PyObject *args)
{
  MDB_txn * x0;
  unsigned int x1;
  MDB_val * x2;
  MDB_val * x3;
  Py_ssize_t datasize;
  int result;
  PyObject *arg0;
  PyObject *arg1;
  PyObject *arg2;
  PyObject *arg3;

  if (!PyArg_ParseTuple(args, "OOOO:mdb_get", &arg0, &arg1, &arg2, &arg3))
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(4), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = alloca((size_t)datasize);
    memset((void *)x0, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(4), arg0) < 0)
      return NULL;
  }

  x1 = _cffi_to_c_int(arg1, unsigned int);
  if (x1 == (unsigned int)-1 && PyErr_Occurred())
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(3), arg2, (char **)&x2);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x2 = alloca((size_t)datasize);
    memset((void *)x2, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x2, _cffi_type(3), arg2) < 0)
      return NULL;
  }

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(3), arg3, (char **)&x3);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x3 = alloca((size_t)datasize);
    memset((void *)x3, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x3, _cffi_type(3), arg3) < 0)
      return NULL;
  }

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = mdb_get(x0, x1, x2, x3); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  return _cffi_from_c_int(result, int);
}

static PyObject *
_cffi_f_mdb_reader_check(PyObject *self, PyObject *args)
{
  MDB_env * x0;
  int * x1;
  Py_ssize_t datasize;
  int result;
  PyObject *arg0;
  PyObject *arg1;

  if (!PyArg_ParseTuple(args, "OO:mdb_reader_check", &arg0, &arg1))
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(8), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = alloca((size_t)datasize);
    memset((void *)x0, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(8), arg0) < 0)
      return NULL;
  }

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(13), arg1, (char **)&x1);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x1 = alloca((size_t)datasize);
    memset((void *)x1, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x1, _cffi_type(13), arg1) < 0)
      return NULL;
  }

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = mdb_reader_check(x0, x1); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  return _cffi_from_c_int(result, int);
}

static PyObject *
_cffi_f_mdb_reader_list(PyObject *self, PyObject *args)
{
  MDB_env * x0;
  int(* x1)(char const *, void *);
  void * x2;
  Py_ssize_t datasize;
  int result;
  PyObject *arg0;
  PyObject *arg1;
  PyObject *arg2;

  if (!PyArg_ParseTuple(args, "OOO:mdb_reader_list", &arg0, &arg1, &arg2))
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(8), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = alloca((size_t)datasize);
    memset((void *)x0, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(8), arg0) < 0)
      return NULL;
  }

  x1 = (int(*)(char const *, void *))_cffi_to_c_pointer(arg1, _cffi_type(14));
  if (x1 == (int(*)(char const *, void *))NULL && PyErr_Occurred())
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(15), arg2, (char **)&x2);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x2 = alloca((size_t)datasize);
    memset((void *)x2, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x2, _cffi_type(15), arg2) < 0)
      return NULL;
  }

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = mdb_reader_list(x0, x1, x2); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  return _cffi_from_c_int(result, int);
}

static PyObject *
_cffi_f_mdb_stat(PyObject *self, PyObject *args)
{
  MDB_txn * x0;
  unsigned int x1;
  MDB_stat * x2;
  Py_ssize_t datasize;
  int result;
  PyObject *arg0;
  PyObject *arg1;
  PyObject *arg2;

  if (!PyArg_ParseTuple(args, "OOO:mdb_stat", &arg0, &arg1, &arg2))
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(4), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = alloca((size_t)datasize);
    memset((void *)x0, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(4), arg0) < 0)
      return NULL;
  }

  x1 = _cffi_to_c_int(arg1, unsigned int);
  if (x1 == (unsigned int)-1 && PyErr_Occurred())
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(12), arg2, (char **)&x2);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x2 = alloca((size_t)datasize);
    memset((void *)x2, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x2, _cffi_type(12), arg2) < 0)
      return NULL;
  }

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = mdb_stat(x0, x1, x2); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  return _cffi_from_c_int(result, int);
}

static PyObject *
_cffi_f_mdb_strerror(PyObject *self, PyObject *arg0)
{
  int x0;
  char * result;

  x0 = _cffi_to_c_int(arg0, int);
  if (x0 == (int)-1 && PyErr_Occurred())
    return NULL;

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = mdb_strerror(x0); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  return _cffi_from_c_pointer((char *)result, _cffi_type(16));
}

static PyObject *
_cffi_f_mdb_txn_abort(PyObject *self, PyObject *arg0)
{
  MDB_txn * x0;
  Py_ssize_t datasize;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(4), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = alloca((size_t)datasize);
    memset((void *)x0, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(4), arg0) < 0)
      return NULL;
  }

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { mdb_txn_abort(x0); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  Py_INCREF(Py_None);
  return Py_None;
}

static PyObject *
_cffi_f_mdb_txn_begin(PyObject *self, PyObject *args)
{
  MDB_env * x0;
  MDB_txn * x1;
  unsigned int x2;
  MDB_txn * * x3;
  Py_ssize_t datasize;
  int result;
  PyObject *arg0;
  PyObject *arg1;
  PyObject *arg2;
  PyObject *arg3;

  if (!PyArg_ParseTuple(args, "OOOO:mdb_txn_begin", &arg0, &arg1, &arg2, &arg3))
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(8), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = alloca((size_t)datasize);
    memset((void *)x0, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(8), arg0) < 0)
      return NULL;
  }

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(4), arg1, (char **)&x1);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x1 = alloca((size_t)datasize);
    memset((void *)x1, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x1, _cffi_type(4), arg1) < 0)
      return NULL;
  }

  x2 = _cffi_to_c_int(arg2, unsigned int);
  if (x2 == (unsigned int)-1 && PyErr_Occurred())
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(17), arg3, (char **)&x3);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x3 = alloca((size_t)datasize);
    memset((void *)x3, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x3, _cffi_type(17), arg3) < 0)
      return NULL;
  }

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = mdb_txn_begin(x0, x1, x2, x3); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  return _cffi_from_c_int(result, int);
}

static PyObject *
_cffi_f_mdb_txn_commit(PyObject *self, PyObject *arg0)
{
  MDB_txn * x0;
  Py_ssize_t datasize;
  int result;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(4), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = alloca((size_t)datasize);
    memset((void *)x0, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(4), arg0) < 0)
      return NULL;
  }

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = mdb_txn_commit(x0); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  return _cffi_from_c_int(result, int);
}

static PyObject *
_cffi_f_mdb_txn_id(PyObject *self, PyObject *arg0)
{
  MDB_txn * x0;
  Py_ssize_t datasize;
  size_t result;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(4), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = alloca((size_t)datasize);
    memset((void *)x0, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(4), arg0) < 0)
      return NULL;
  }

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = mdb_txn_id(x0); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  return _cffi_from_c_int(result, size_t);
}

static PyObject *
_cffi_f_mdb_txn_renew(PyObject *self, PyObject *arg0)
{
  MDB_txn * x0;
  Py_ssize_t datasize;
  int result;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(4), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = alloca((size_t)datasize);
    memset((void *)x0, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(4), arg0) < 0)
      return NULL;
  }

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = mdb_txn_renew(x0); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  return _cffi_from_c_int(result, int);
}

static PyObject *
_cffi_f_mdb_txn_reset(PyObject *self, PyObject *arg0)
{
  MDB_txn * x0;
  Py_ssize_t datasize;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(4), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = alloca((size_t)datasize);
    memset((void *)x0, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(4), arg0) < 0)
      return NULL;
  }

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { mdb_txn_reset(x0); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  Py_INCREF(Py_None);
  return Py_None;
}

static PyObject *
_cffi_f_pymdb_cursor_get(PyObject *self, PyObject *args)
{
  MDB_cursor * x0;
  char * x1;
  size_t x2;
  char * x3;
  size_t x4;
  MDB_val * x5;
  MDB_val * x6;
  int x7;
  Py_ssize_t datasize;
  int result;
  PyObject *arg0;
  PyObject *arg1;
  PyObject *arg2;
  PyObject *arg3;
  PyObject *arg4;
  PyObject *arg5;
  PyObject *arg6;
  PyObject *arg7;

  if (!PyArg_ParseTuple(args, "OOOOOOOO:pymdb_cursor_get", &arg0, &arg1, &arg2, &arg3, &arg4, &arg5, &arg6, &arg7))
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(0), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = alloca((size_t)datasize);
    memset((void *)x0, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(0), arg0) < 0)
      return NULL;
  }

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(16), arg1, (char **)&x1);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x1 = alloca((size_t)datasize);
    memset((void *)x1, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x1, _cffi_type(16), arg1) < 0)
      return NULL;
  }

  x2 = _cffi_to_c_int(arg2, size_t);
  if (x2 == (size_t)-1 && PyErr_Occurred())
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(16), arg3, (char **)&x3);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x3 = alloca((size_t)datasize);
    memset((void *)x3, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x3, _cffi_type(16), arg3) < 0)
      return NULL;
  }

  x4 = _cffi_to_c_int(arg4, size_t);
  if (x4 == (size_t)-1 && PyErr_Occurred())
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(3), arg5, (char **)&x5);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x5 = alloca((size_t)datasize);
    memset((void *)x5, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x5, _cffi_type(3), arg5) < 0)
      return NULL;
  }

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(3), arg6, (char **)&x6);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x6 = alloca((size_t)datasize);
    memset((void *)x6, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x6, _cffi_type(3), arg6) < 0)
      return NULL;
  }

  x7 = _cffi_to_c_int(arg7, int);
  if (x7 == (int)-1 && PyErr_Occurred())
    return NULL;

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = pymdb_cursor_get(x0, x1, x2, x3, x4, x5, x6, x7); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  return _cffi_from_c_int(result, int);
}

static PyObject *
_cffi_f_pymdb_cursor_put(PyObject *self, PyObject *args)
{
  MDB_cursor * x0;
  char * x1;
  size_t x2;
  char * x3;
  size_t x4;
  int x5;
  Py_ssize_t datasize;
  int result;
  PyObject *arg0;
  PyObject *arg1;
  PyObject *arg2;
  PyObject *arg3;
  PyObject *arg4;
  PyObject *arg5;

  if (!PyArg_ParseTuple(args, "OOOOOO:pymdb_cursor_put", &arg0, &arg1, &arg2, &arg3, &arg4, &arg5))
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(0), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = alloca((size_t)datasize);
    memset((void *)x0, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(0), arg0) < 0)
      return NULL;
  }

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(16), arg1, (char **)&x1);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x1 = alloca((size_t)datasize);
    memset((void *)x1, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x1, _cffi_type(16), arg1) < 0)
      return NULL;
  }

  x2 = _cffi_to_c_int(arg2, size_t);
  if (x2 == (size_t)-1 && PyErr_Occurred())
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(16), arg3, (char **)&x3);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x3 = alloca((size_t)datasize);
    memset((void *)x3, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x3, _cffi_type(16), arg3) < 0)
      return NULL;
  }

  x4 = _cffi_to_c_int(arg4, size_t);
  if (x4 == (size_t)-1 && PyErr_Occurred())
    return NULL;

  x5 = _cffi_to_c_int(arg5, int);
  if (x5 == (int)-1 && PyErr_Occurred())
    return NULL;

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = pymdb_cursor_put(x0, x1, x2, x3, x4, x5); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  return _cffi_from_c_int(result, int);
}

static PyObject *
_cffi_f_pymdb_del(PyObject *self, PyObject *args)
{
  MDB_txn * x0;
  unsigned int x1;
  char * x2;
  size_t x3;
  char * x4;
  size_t x5;
  Py_ssize_t datasize;
  int result;
  PyObject *arg0;
  PyObject *arg1;
  PyObject *arg2;
  PyObject *arg3;
  PyObject *arg4;
  PyObject *arg5;

  if (!PyArg_ParseTuple(args, "OOOOOO:pymdb_del", &arg0, &arg1, &arg2, &arg3, &arg4, &arg5))
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(4), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = alloca((size_t)datasize);
    memset((void *)x0, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(4), arg0) < 0)
      return NULL;
  }

  x1 = _cffi_to_c_int(arg1, unsigned int);
  if (x1 == (unsigned int)-1 && PyErr_Occurred())
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(16), arg2, (char **)&x2);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x2 = alloca((size_t)datasize);
    memset((void *)x2, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x2, _cffi_type(16), arg2) < 0)
      return NULL;
  }

  x3 = _cffi_to_c_int(arg3, size_t);
  if (x3 == (size_t)-1 && PyErr_Occurred())
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(16), arg4, (char **)&x4);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x4 = alloca((size_t)datasize);
    memset((void *)x4, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x4, _cffi_type(16), arg4) < 0)
      return NULL;
  }

  x5 = _cffi_to_c_int(arg5, size_t);
  if (x5 == (size_t)-1 && PyErr_Occurred())
    return NULL;

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = pymdb_del(x0, x1, x2, x3, x4, x5); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  return _cffi_from_c_int(result, int);
}

static PyObject *
_cffi_f_pymdb_get(PyObject *self, PyObject *args)
{
  MDB_txn * x0;
  unsigned int x1;
  char * x2;
  size_t x3;
  MDB_val * x4;
  Py_ssize_t datasize;
  int result;
  PyObject *arg0;
  PyObject *arg1;
  PyObject *arg2;
  PyObject *arg3;
  PyObject *arg4;

  if (!PyArg_ParseTuple(args, "OOOOO:pymdb_get", &arg0, &arg1, &arg2, &arg3, &arg4))
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(4), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = alloca((size_t)datasize);
    memset((void *)x0, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(4), arg0) < 0)
      return NULL;
  }

  x1 = _cffi_to_c_int(arg1, unsigned int);
  if (x1 == (unsigned int)-1 && PyErr_Occurred())
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(16), arg2, (char **)&x2);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x2 = alloca((size_t)datasize);
    memset((void *)x2, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x2, _cffi_type(16), arg2) < 0)
      return NULL;
  }

  x3 = _cffi_to_c_int(arg3, size_t);
  if (x3 == (size_t)-1 && PyErr_Occurred())
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(3), arg4, (char **)&x4);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x4 = alloca((size_t)datasize);
    memset((void *)x4, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x4, _cffi_type(3), arg4) < 0)
      return NULL;
  }

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = pymdb_get(x0, x1, x2, x3, x4); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  return _cffi_from_c_int(result, int);
}

static PyObject *
_cffi_f_pymdb_put(PyObject *self, PyObject *args)
{
  MDB_txn * x0;
  unsigned int x1;
  char * x2;
  size_t x3;
  char * x4;
  size_t x5;
  unsigned int x6;
  Py_ssize_t datasize;
  int result;
  PyObject *arg0;
  PyObject *arg1;
  PyObject *arg2;
  PyObject *arg3;
  PyObject *arg4;
  PyObject *arg5;
  PyObject *arg6;

  if (!PyArg_ParseTuple(args, "OOOOOOO:pymdb_put", &arg0, &arg1, &arg2, &arg3, &arg4, &arg5, &arg6))
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(4), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = alloca((size_t)datasize);
    memset((void *)x0, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(4), arg0) < 0)
      return NULL;
  }

  x1 = _cffi_to_c_int(arg1, unsigned int);
  if (x1 == (unsigned int)-1 && PyErr_Occurred())
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(16), arg2, (char **)&x2);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x2 = alloca((size_t)datasize);
    memset((void *)x2, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x2, _cffi_type(16), arg2) < 0)
      return NULL;
  }

  x3 = _cffi_to_c_int(arg3, size_t);
  if (x3 == (size_t)-1 && PyErr_Occurred())
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(16), arg4, (char **)&x4);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x4 = alloca((size_t)datasize);
    memset((void *)x4, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x4, _cffi_type(16), arg4) < 0)
      return NULL;
  }

  x5 = _cffi_to_c_int(arg5, size_t);
  if (x5 == (size_t)-1 && PyErr_Occurred())
    return NULL;

  x6 = _cffi_to_c_int(arg6, unsigned int);
  if (x6 == (unsigned int)-1 && PyErr_Occurred())
    return NULL;

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = pymdb_put(x0, x1, x2, x3, x4, x5, x6); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  return _cffi_from_c_int(result, int);
}

static int _cffi_const_EACCES(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(EACCES);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "EACCES", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return ((void)lib,0);
}

static int _cffi_const_EAGAIN(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(EAGAIN);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "EAGAIN", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_EACCES(lib);
}

static int _cffi_const_EINVAL(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(EINVAL);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "EINVAL", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_EAGAIN(lib);
}

static int _cffi_const_ENOMEM(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(ENOMEM);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "ENOMEM", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_EINVAL(lib);
}

static int _cffi_const_ENOSPC(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(ENOSPC);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "ENOSPC", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_ENOMEM(lib);
}

static int _cffi_const_MDB_APPEND(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(MDB_APPEND);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "MDB_APPEND", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_ENOSPC(lib);
}

static int _cffi_const_MDB_BAD_DBI(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(MDB_BAD_DBI);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "MDB_BAD_DBI", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_MDB_APPEND(lib);
}

static int _cffi_const_MDB_BAD_RSLOT(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(MDB_BAD_RSLOT);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "MDB_BAD_RSLOT", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_MDB_BAD_DBI(lib);
}

static int _cffi_const_MDB_BAD_TXN(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(MDB_BAD_TXN);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "MDB_BAD_TXN", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_MDB_BAD_RSLOT(lib);
}

static int _cffi_const_MDB_BAD_VALSIZE(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(MDB_BAD_VALSIZE);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "MDB_BAD_VALSIZE", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_MDB_BAD_TXN(lib);
}

static int _cffi_const_MDB_CORRUPTED(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(MDB_CORRUPTED);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "MDB_CORRUPTED", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_MDB_BAD_VALSIZE(lib);
}

static int _cffi_const_MDB_CP_COMPACT(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(MDB_CP_COMPACT);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "MDB_CP_COMPACT", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_MDB_CORRUPTED(lib);
}

static int _cffi_const_MDB_CREATE(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(MDB_CREATE);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "MDB_CREATE", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_MDB_CP_COMPACT(lib);
}

static int _cffi_const_MDB_CURSOR_FULL(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(MDB_CURSOR_FULL);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "MDB_CURSOR_FULL", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_MDB_CREATE(lib);
}

static int _cffi_const_MDB_DBS_FULL(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(MDB_DBS_FULL);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "MDB_DBS_FULL", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_MDB_CURSOR_FULL(lib);
}

static int _cffi_const_MDB_DUPFIXED(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(MDB_DUPFIXED);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "MDB_DUPFIXED", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_MDB_DBS_FULL(lib);
}

static int _cffi_const_MDB_DUPSORT(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(MDB_DUPSORT);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "MDB_DUPSORT", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_MDB_DUPFIXED(lib);
}

static int _cffi_const_MDB_INCOMPATIBLE(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(MDB_INCOMPATIBLE);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "MDB_INCOMPATIBLE", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_MDB_DUPSORT(lib);
}

static int _cffi_const_MDB_INTEGERDUP(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(MDB_INTEGERDUP);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "MDB_INTEGERDUP", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_MDB_INCOMPATIBLE(lib);
}

static int _cffi_const_MDB_INTEGERKEY(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(MDB_INTEGERKEY);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "MDB_INTEGERKEY", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_MDB_INTEGERDUP(lib);
}

static int _cffi_const_MDB_INVALID(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(MDB_INVALID);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "MDB_INVALID", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_MDB_INTEGERKEY(lib);
}

static int _cffi_const_MDB_KEYEXIST(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(MDB_KEYEXIST);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "MDB_KEYEXIST", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_MDB_INVALID(lib);
}

static int _cffi_const_MDB_MAPASYNC(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(MDB_MAPASYNC);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "MDB_MAPASYNC", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_MDB_KEYEXIST(lib);
}

static int _cffi_const_MDB_MAP_FULL(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(MDB_MAP_FULL);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "MDB_MAP_FULL", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_MDB_MAPASYNC(lib);
}

static int _cffi_const_MDB_MAP_RESIZED(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(MDB_MAP_RESIZED);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "MDB_MAP_RESIZED", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_MDB_MAP_FULL(lib);
}

static int _cffi_const_MDB_NODUPDATA(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(MDB_NODUPDATA);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "MDB_NODUPDATA", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_MDB_MAP_RESIZED(lib);
}

static int _cffi_const_MDB_NOLOCK(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(MDB_NOLOCK);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "MDB_NOLOCK", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_MDB_NODUPDATA(lib);
}

static int _cffi_const_MDB_NOMEMINIT(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(MDB_NOMEMINIT);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "MDB_NOMEMINIT", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_MDB_NOLOCK(lib);
}

static int _cffi_const_MDB_NOMETASYNC(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(MDB_NOMETASYNC);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "MDB_NOMETASYNC", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_MDB_NOMEMINIT(lib);
}

static int _cffi_const_MDB_NOOVERWRITE(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(MDB_NOOVERWRITE);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "MDB_NOOVERWRITE", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_MDB_NOMETASYNC(lib);
}

static int _cffi_const_MDB_NORDAHEAD(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(MDB_NORDAHEAD);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "MDB_NORDAHEAD", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_MDB_NOOVERWRITE(lib);
}

static int _cffi_const_MDB_NOSUBDIR(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(MDB_NOSUBDIR);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "MDB_NOSUBDIR", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_MDB_NORDAHEAD(lib);
}

static int _cffi_const_MDB_NOSYNC(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(MDB_NOSYNC);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "MDB_NOSYNC", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_MDB_NOSUBDIR(lib);
}

static int _cffi_const_MDB_NOTFOUND(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(MDB_NOTFOUND);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "MDB_NOTFOUND", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_MDB_NOSYNC(lib);
}

static int _cffi_const_MDB_NOTLS(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(MDB_NOTLS);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "MDB_NOTLS", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_MDB_NOTFOUND(lib);
}

static int _cffi_const_MDB_PAGE_FULL(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(MDB_PAGE_FULL);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "MDB_PAGE_FULL", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_MDB_NOTLS(lib);
}

static int _cffi_const_MDB_PAGE_NOTFOUND(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(MDB_PAGE_NOTFOUND);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "MDB_PAGE_NOTFOUND", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_MDB_PAGE_FULL(lib);
}

static int _cffi_const_MDB_PANIC(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(MDB_PANIC);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "MDB_PANIC", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_MDB_PAGE_NOTFOUND(lib);
}

static int _cffi_const_MDB_RDONLY(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(MDB_RDONLY);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "MDB_RDONLY", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_MDB_PANIC(lib);
}

static int _cffi_const_MDB_READERS_FULL(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(MDB_READERS_FULL);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "MDB_READERS_FULL", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_MDB_RDONLY(lib);
}

static int _cffi_const_MDB_REVERSEKEY(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(MDB_REVERSEKEY);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "MDB_REVERSEKEY", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_MDB_READERS_FULL(lib);
}

static int _cffi_const_MDB_TLS_FULL(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(MDB_TLS_FULL);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "MDB_TLS_FULL", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_MDB_REVERSEKEY(lib);
}

static int _cffi_const_MDB_TXN_FULL(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(MDB_TXN_FULL);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "MDB_TXN_FULL", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_MDB_TLS_FULL(lib);
}

static int _cffi_const_MDB_VERSION_MAJOR(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(MDB_VERSION_MAJOR);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "MDB_VERSION_MAJOR", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_MDB_TXN_FULL(lib);
}

static int _cffi_const_MDB_VERSION_MINOR(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(MDB_VERSION_MINOR);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "MDB_VERSION_MINOR", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_MDB_VERSION_MAJOR(lib);
}

static int _cffi_const_MDB_VERSION_MISMATCH(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(MDB_VERSION_MISMATCH);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "MDB_VERSION_MISMATCH", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_MDB_VERSION_MINOR(lib);
}

static int _cffi_const_MDB_VERSION_PATCH(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(MDB_VERSION_PATCH);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "MDB_VERSION_PATCH", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_MDB_VERSION_MISMATCH(lib);
}

static int _cffi_const_MDB_WRITEMAP(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(MDB_WRITEMAP);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "MDB_WRITEMAP", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_MDB_VERSION_PATCH(lib);
}

static void _cffi_check_struct_MDB_envinfo(struct MDB_envinfo *p)
{
  /* only to generate compile-time warnings or errors */
  (void)p;
  { void * *tmp = &p->me_mapaddr; (void)tmp; }
  (void)((p->me_mapsize) << 1);
  (void)((p->me_last_pgno) << 1);
  (void)((p->me_last_txnid) << 1);
  (void)((p->me_maxreaders) << 1);
  (void)((p->me_numreaders) << 1);
}
static PyObject *
_cffi_layout_struct_MDB_envinfo(PyObject *self, PyObject *noarg)
{
  struct _cffi_aligncheck { char x; struct MDB_envinfo y; };
  static Py_ssize_t nums[] = {
    sizeof(struct MDB_envinfo),
    offsetof(struct _cffi_aligncheck, y),
    offsetof(struct MDB_envinfo, me_mapaddr),
    sizeof(((struct MDB_envinfo *)0)->me_mapaddr),
    offsetof(struct MDB_envinfo, me_mapsize),
    sizeof(((struct MDB_envinfo *)0)->me_mapsize),
    offsetof(struct MDB_envinfo, me_last_pgno),
    sizeof(((struct MDB_envinfo *)0)->me_last_pgno),
    offsetof(struct MDB_envinfo, me_last_txnid),
    sizeof(((struct MDB_envinfo *)0)->me_last_txnid),
    offsetof(struct MDB_envinfo, me_maxreaders),
    sizeof(((struct MDB_envinfo *)0)->me_maxreaders),
    offsetof(struct MDB_envinfo, me_numreaders),
    sizeof(((struct MDB_envinfo *)0)->me_numreaders),
    -1
  };
  (void)self; /* unused */
  (void)noarg; /* unused */
  return _cffi_get_struct_layout(nums);
  /* the next line is not executed, but compiled */
  _cffi_check_struct_MDB_envinfo(0);
}

static void _cffi_check_struct_MDB_stat(struct MDB_stat *p)
{
  /* only to generate compile-time warnings or errors */
  (void)p;
  (void)((p->ms_psize) << 1);
  (void)((p->ms_depth) << 1);
  (void)((p->ms_branch_pages) << 1);
  (void)((p->ms_leaf_pages) << 1);
  (void)((p->ms_overflow_pages) << 1);
  (void)((p->ms_entries) << 1);
}
static PyObject *
_cffi_layout_struct_MDB_stat(PyObject *self, PyObject *noarg)
{
  struct _cffi_aligncheck { char x; struct MDB_stat y; };
  static Py_ssize_t nums[] = {
    sizeof(struct MDB_stat),
    offsetof(struct _cffi_aligncheck, y),
    offsetof(struct MDB_stat, ms_psize),
    sizeof(((struct MDB_stat *)0)->ms_psize),
    offsetof(struct MDB_stat, ms_depth),
    sizeof(((struct MDB_stat *)0)->ms_depth),
    offsetof(struct MDB_stat, ms_branch_pages),
    sizeof(((struct MDB_stat *)0)->ms_branch_pages),
    offsetof(struct MDB_stat, ms_leaf_pages),
    sizeof(((struct MDB_stat *)0)->ms_leaf_pages),
    offsetof(struct MDB_stat, ms_overflow_pages),
    sizeof(((struct MDB_stat *)0)->ms_overflow_pages),
    offsetof(struct MDB_stat, ms_entries),
    sizeof(((struct MDB_stat *)0)->ms_entries),
    -1
  };
  (void)self; /* unused */
  (void)noarg; /* unused */
  return _cffi_get_struct_layout(nums);
  /* the next line is not executed, but compiled */
  _cffi_check_struct_MDB_stat(0);
}

static void _cffi_check_struct_MDB_val(struct MDB_val *p)
{
  /* only to generate compile-time warnings or errors */
  (void)p;
  (void)((p->mv_size) << 1);
  { void * *tmp = &p->mv_data; (void)tmp; }
}
static PyObject *
_cffi_layout_struct_MDB_val(PyObject *self, PyObject *noarg)
{
  struct _cffi_aligncheck { char x; struct MDB_val y; };
  static Py_ssize_t nums[] = {
    sizeof(struct MDB_val),
    offsetof(struct _cffi_aligncheck, y),
    offsetof(struct MDB_val, mv_size),
    sizeof(((struct MDB_val *)0)->mv_size),
    offsetof(struct MDB_val, mv_data),
    sizeof(((struct MDB_val *)0)->mv_data),
    -1
  };
  (void)self; /* unused */
  (void)noarg; /* unused */
  return _cffi_get_struct_layout(nums);
  /* the next line is not executed, but compiled */
  _cffi_check_struct_MDB_val(0);
}

static int _cffi_setup_custom(PyObject *lib)
{
  return _cffi_const_MDB_WRITEMAP(lib);
}

static PyMethodDef _cffi_methods[] = {
  {"mdb_cursor_close", _cffi_f_mdb_cursor_close, METH_O, NULL},
  {"mdb_cursor_count", _cffi_f_mdb_cursor_count, METH_VARARGS, NULL},
  {"mdb_cursor_del", _cffi_f_mdb_cursor_del, METH_VARARGS, NULL},
  {"mdb_cursor_get", _cffi_f_mdb_cursor_get, METH_VARARGS, NULL},
  {"mdb_cursor_open", _cffi_f_mdb_cursor_open, METH_VARARGS, NULL},
  {"mdb_dbi_flags", _cffi_f_mdb_dbi_flags, METH_VARARGS, NULL},
  {"mdb_dbi_open", _cffi_f_mdb_dbi_open, METH_VARARGS, NULL},
  {"mdb_drop", _cffi_f_mdb_drop, METH_VARARGS, NULL},
  {"mdb_env_close", _cffi_f_mdb_env_close, METH_O, NULL},
  {"mdb_env_copy2", _cffi_f_mdb_env_copy2, METH_VARARGS, NULL},
  {"mdb_env_copyfd2", _cffi_f_mdb_env_copyfd2, METH_VARARGS, NULL},
  {"mdb_env_create", _cffi_f_mdb_env_create, METH_O, NULL},
  {"mdb_env_get_flags", _cffi_f_mdb_env_get_flags, METH_VARARGS, NULL},
  {"mdb_env_get_maxkeysize", _cffi_f_mdb_env_get_maxkeysize, METH_O, NULL},
  {"mdb_env_get_maxreaders", _cffi_f_mdb_env_get_maxreaders, METH_VARARGS, NULL},
  {"mdb_env_get_path", _cffi_f_mdb_env_get_path, METH_VARARGS, NULL},
  {"mdb_env_info", _cffi_f_mdb_env_info, METH_VARARGS, NULL},
  {"mdb_env_open", _cffi_f_mdb_env_open, METH_VARARGS, NULL},
  {"mdb_env_set_flags", _cffi_f_mdb_env_set_flags, METH_VARARGS, NULL},
  {"mdb_env_set_mapsize", _cffi_f_mdb_env_set_mapsize, METH_VARARGS, NULL},
  {"mdb_env_set_maxdbs", _cffi_f_mdb_env_set_maxdbs, METH_VARARGS, NULL},
  {"mdb_env_set_maxreaders", _cffi_f_mdb_env_set_maxreaders, METH_VARARGS, NULL},
  {"mdb_env_stat", _cffi_f_mdb_env_stat, METH_VARARGS, NULL},
  {"mdb_env_sync", _cffi_f_mdb_env_sync, METH_VARARGS, NULL},
  {"mdb_get", _cffi_f_mdb_get, METH_VARARGS, NULL},
  {"mdb_reader_check", _cffi_f_mdb_reader_check, METH_VARARGS, NULL},
  {"mdb_reader_list", _cffi_f_mdb_reader_list, METH_VARARGS, NULL},
  {"mdb_stat", _cffi_f_mdb_stat, METH_VARARGS, NULL},
  {"mdb_strerror", _cffi_f_mdb_strerror, METH_O, NULL},
  {"mdb_txn_abort", _cffi_f_mdb_txn_abort, METH_O, NULL},
  {"mdb_txn_begin", _cffi_f_mdb_txn_begin, METH_VARARGS, NULL},
  {"mdb_txn_commit", _cffi_f_mdb_txn_commit, METH_O, NULL},
  {"mdb_txn_id", _cffi_f_mdb_txn_id, METH_O, NULL},
  {"mdb_txn_renew", _cffi_f_mdb_txn_renew, METH_O, NULL},
  {"mdb_txn_reset", _cffi_f_mdb_txn_reset, METH_O, NULL},
  {"pymdb_cursor_get", _cffi_f_pymdb_cursor_get, METH_VARARGS, NULL},
  {"pymdb_cursor_put", _cffi_f_pymdb_cursor_put, METH_VARARGS, NULL},
  {"pymdb_del", _cffi_f_pymdb_del, METH_VARARGS, NULL},
  {"pymdb_get", _cffi_f_pymdb_get, METH_VARARGS, NULL},
  {"pymdb_put", _cffi_f_pymdb_put, METH_VARARGS, NULL},
  {"_cffi_layout_struct_MDB_envinfo", _cffi_layout_struct_MDB_envinfo, METH_NOARGS, NULL},
  {"_cffi_layout_struct_MDB_stat", _cffi_layout_struct_MDB_stat, METH_NOARGS, NULL},
  {"_cffi_layout_struct_MDB_val", _cffi_layout_struct_MDB_val, METH_NOARGS, NULL},
  {"_cffi_setup", _cffi_setup, METH_VARARGS, NULL},
  {NULL, NULL, 0, NULL}    /* Sentinel */
};

#if PY_MAJOR_VERSION >= 3

static struct PyModuleDef _cffi_module_def = {
  PyModuleDef_HEAD_INIT,
  "lmdb_cffi",
  NULL,
  -1,
  _cffi_methods,
  NULL, NULL, NULL, NULL
};

PyMODINIT_FUNC
PyInit_lmdb_cffi(void)
{
  PyObject *lib;
  lib = PyModule_Create(&_cffi_module_def);
  if (lib == NULL)
    return NULL;
  if (_cffi_const_MDB_SET_RANGE(lib) < 0 || _cffi_init() < 0) {
    Py_DECREF(lib);
    return NULL;
  }
  return lib;
}

#else

PyMODINIT_FUNC
initlmdb_cffi(void)
{
  PyObject *lib;
  lib = Py_InitModule("lmdb_cffi", _cffi_methods);
  if (lib == NULL)
    return;
  if (_cffi_const_MDB_SET_RANGE(lib) < 0 || _cffi_init() < 0)
    return;
  return;
}

#endif
