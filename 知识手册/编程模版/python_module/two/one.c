
#define PY_SSIZE_T_CLEAN
#include <Python.h>

PyDoc_STRVAR(_doc, "this is test\n");
static PyObject* test_help() {
    return PyUnicode_FromString(_doc);
}


static PyMethodDef test_methods[] = {
    {"help", (PyCFunction)test_help, METH_NOARGS, _doc},
    {NULL, NULL, 0, NULL}
};

static struct PyModuleDef test_module = {
    "test",
    _MAIX_NN_NAME_,         /* Module name */
    _doc,	/* Module _maixMethods */
    -1,			    /* size of per-interpreter state of the module, size of per-interpreter state of the module,*/
    test_methods,
};

PyMODINIT_FUNC PyInit__test(void)
{

    PyObject *module;

    module = PyModule_Create(&test_module);
    PyObject *version = PyUnicode_FromString("0.0.1");


    /* Set module version */
    PyObject *dict = PyModule_GetDict(module);
    PyDict_SetItemString(dict, "__version__", version);
    Py_DECREF(version);

    /* Add class*/
    // PyModule_AddObject(module, "Model", SpamError);

    /* init object(default value) */
    // if (PyType_Ready(&one_mode_type) < 0) {
    //     return NULL;
    // }



    /* Add maix.nn.decoder module,
       add maix.nn.decoder.Yolo2 class*/
    PyObject *decoder_module = PyModule_Create(&one_mode);
    Py_INCREF(&one_Type);
    PyModule_AddObject(decoder_module, "Yolo2", (PyObject*)&one_Type);
    PyModule_AddObject(module, "decoder", decoder_module);


    return module;
}
