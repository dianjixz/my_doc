
#include "Python.h"

typedef struct
{
    PyObject_HEAD;
    bool init;
} DObject;

PyDoc_STRVAR(_type_doc, "neural network model Yolo2 decoder object.\n");

static int max_index(float *a, int n)
{
    int i, max_i = 0;
    float max = a[0];

    for (i = 1; i < n; ++i)
    {
        if (a[i] > max)
        {
            max = a[i];
            max_i = i;
        }
    }
    return max_i;
}

static int _2_init(DObject *self, PyObject *args, PyObject *kwds);

static PyObject* _2_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
    DObject* self = (DObject*)type->tp_alloc(type, 0);
    if (!self)
    {
        PyErr_NoMemory();
        return NULL;
    }
    return (PyObject*)self;
}


static void _2_del(DObject *self)
{
    self->init = false;
}

static int _2_init(DObject *self, PyObject *args, PyObject *kwds)
{
    if(self->init)
    {
        PyErr_SetString(PyExc_Exception, "already initialized");
        return -1;
    }
    self->init = true;
    return 0;
}

static PyObject *_2_str(PyObject *object)
{
  char desc[] = "Yolo2 decoder object";
  PyObject *dev_desc = NULL;

  dev_desc = PyUnicode_FromString(desc);

  return dev_desc;
}

PyDoc_STRVAR(_run_doc, "run decode process.\n");
static PyObject* _run(DecoderYolo2Object *self, PyObject *args, PyObject *kw_args)
{
    if(!self->init)
    {
        PyErr_SetString(PyExc_Exception, "not initialize");
        return NULL;
    }

    PyObject* final_result = PyList_New(0);
    return final_result;
}



static PyMethodDef _2_methods[] = {
    {"run", (PyCFunction)_run, METH_VARARGS, _run_doc},
    {NULL, NULL, 0, NULL},
};


PyTypeObject one_Type = {
    PyVarObject_HEAD_INIT(NULL, 0)
    _2_type_name,                  /* tp_name */
    sizeof(DObject),                      /* tp_basicsize */
    0,                                        /* tp_itemsize */
    (destructor)_2_del,                    /* tp_dealloc */
    0,                                        /* tp_print */
    0,                                        /* tp_getattr */
    0,                                        /* tp_setattr */
    0,                                        /* tp_compare */
    0,                                        /* tp_repr */
    0,                                        /* tp_as_number */
    0,                                        /* tp_as_sequence */
    0,                                        /* tp_as_mapping */
    0,                                        /* tp_hash */
    0,                                        /* tp_call */
    _2_str,                                /* tp_str */
    0,                                        /* tp_getattro */
    0,                                        /* tp_setattro */
    0,                                        /* tp_as_buffer */
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE, /* tp_flags */
    _type_doc,                  /* tp_doc */
    0,                                        /* tp_traverse */
    0,                                        /* tp_clear */
    0,                                        /* tp_richcompare */
    0,                                        /* tp_weaklistoffset */
    0,                                        /* tp_iter */
    0,                                        /* tp_iternext */
    _2_methods,                            /* tp_methods */
    0,                                        /* tp_members */
    0,                                        /* tp_getset */
    0,                                        /* tp_base */
    0,                                        /* tp_dict */
    0,                                        /* tp_descr_get */
    0,                                        /* tp_descr_set */
    0,                                        /* tp_dictoffset */
    (initproc)_2_init,             /* tp_init */
    0,                                        /* tp_alloc */
    _2_new,                        /* tp_new */
};

