/* Generated by Cython 0.28.4 */

#ifndef __PYX_HAVE__gevent__libev__corecext
#define __PYX_HAVE__gevent__libev__corecext

struct PyGeventCallbackObject;
struct PyGeventLoopObject;
struct PyGeventWatcherObject;
struct PyGeventIOObject;
struct PyGeventTimerObject;
struct PyGeventSignalObject;
struct PyGeventIdleObject;
struct PyGeventPrepareObject;
struct PyGeventCheckObject;
struct PyGeventForkObject;
struct PyGeventAsyncObject;
struct PyGeventChildObject;
struct PyGeventStatObject;

struct PyGeventCallbackObject {
  PyObject_HEAD
  PyObject *callback;
  PyObject *args;
  struct PyGeventCallbackObject *next;
};

struct PyGeventLoopObject {
  PyObject_HEAD
  struct __pyx_vtabstruct_6gevent_5libev_8corecext_loop *__pyx_vtab;
  struct ev_prepare _prepare;
  struct ev_timer _timer0;
  struct ev_timer _periodic_signal_checker;
  PyObject *error_handler;
  struct ev_loop *_ptr;
  struct __pyx_obj_6gevent_5libev_8corecext_CallbackFIFO *_callbacks;
  int starting_timer_may_update_loop_time;
  int _default;
};

struct PyGeventWatcherObject {
  PyObject_HEAD
  struct PyGeventLoopObject *loop;
  PyObject *_callback;
  PyObject *args;
  struct ev_watcher *__pyx___watcher;
  struct __pyx_t_6gevent_5libev_8corecext_start_and_stop *__pyx___ss;
  unsigned int _flags;
};

struct PyGeventIOObject {
  struct PyGeventWatcherObject __pyx_base;
  struct ev_io _watcher;
};

struct PyGeventTimerObject {
  struct PyGeventWatcherObject __pyx_base;
  struct ev_timer _watcher;
};

struct PyGeventSignalObject {
  struct PyGeventWatcherObject __pyx_base;
  struct ev_signal _watcher;
};

struct PyGeventIdleObject {
  struct PyGeventWatcherObject __pyx_base;
  struct ev_idle _watcher;
};

struct PyGeventPrepareObject {
  struct PyGeventWatcherObject __pyx_base;
  struct ev_prepare _watcher;
};

struct PyGeventCheckObject {
  struct PyGeventWatcherObject __pyx_base;
  struct ev_check _watcher;
};

struct PyGeventForkObject {
  struct PyGeventWatcherObject __pyx_base;
  struct ev_fork _watcher;
};

struct PyGeventAsyncObject {
  struct PyGeventWatcherObject __pyx_base;
  struct ev_async _watcher;
};

struct PyGeventChildObject {
  struct PyGeventWatcherObject __pyx_base;
  struct ev_child _watcher;
};

struct PyGeventStatObject {
  struct PyGeventWatcherObject __pyx_base;
  struct ev_stat _watcher;
  PyObject *path;
  PyObject *_paths;
};

#ifndef __PYX_HAVE_API__gevent__libev__corecext

#ifndef __PYX_EXTERN_C
  #ifdef __cplusplus
    #define __PYX_EXTERN_C extern "C"
  #else
    #define __PYX_EXTERN_C extern
  #endif
#endif

#ifndef DL_IMPORT
  #define DL_IMPORT(_T) _T
#endif

__PYX_EXTERN_C DL_IMPORT(PyTypeObject) PyGeventCallback_Type;
__PYX_EXTERN_C DL_IMPORT(PyTypeObject) PyGeventLoop_Type;
__PYX_EXTERN_C DL_IMPORT(PyTypeObject) PyGeventWatcher_Type;
__PYX_EXTERN_C DL_IMPORT(PyTypeObject) PyGeventIO_Type;
__PYX_EXTERN_C DL_IMPORT(PyTypeObject) PyGeventTimer_Type;
__PYX_EXTERN_C DL_IMPORT(PyTypeObject) PyGeventSignal_Type;
__PYX_EXTERN_C DL_IMPORT(PyTypeObject) PyGeventIdle_Type;
__PYX_EXTERN_C DL_IMPORT(PyTypeObject) PyGeventPrepare_Type;
__PYX_EXTERN_C DL_IMPORT(PyTypeObject) PyGeventCheck_Type;
__PYX_EXTERN_C DL_IMPORT(PyTypeObject) PyGeventFork_Type;
__PYX_EXTERN_C DL_IMPORT(PyTypeObject) PyGeventAsync_Type;
__PYX_EXTERN_C DL_IMPORT(PyTypeObject) PyGeventChild_Type;
__PYX_EXTERN_C DL_IMPORT(PyTypeObject) PyGeventStat_Type;

__PYX_EXTERN_C void gevent_handle_error(struct PyGeventLoopObject *, PyObject *);
__PYX_EXTERN_C PyObject *gevent_loop_run_callbacks(struct PyGeventLoopObject *);

__PYX_EXTERN_C PyObject *GEVENT_CORE_EVENTS;
__PYX_EXTERN_C PyObject *_empty_tuple;

#endif /* !__PYX_HAVE_API__gevent__libev__corecext */

/* WARNING: the interface of the module init function changed in CPython 3.5. */
/* It now returns a PyModuleDef instance instead of a PyModule instance. */

#if PY_MAJOR_VERSION < 3
PyMODINIT_FUNC initcorecext(void);
#else
PyMODINIT_FUNC PyInit_corecext(void);
#endif

#endif /* !__PYX_HAVE__gevent__libev__corecext */
