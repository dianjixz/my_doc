
#include "Python.h"


PyDoc_STRVAR(r_doc, "one module.\n");
static PyMethodDef r_methods[] = {
    {NULL, NULL, 0, NULL}
};


struct PyModuleDef one_mode = {
    PyModuleDef_HEAD_INIT,
    "decoder",              /* Module name */
    r_doc,	/* Module _maixMethods */
    -1,			    /* size of per-interpreter state of the module, size of per-interpreter state of the module,*/
    r_methods,
};


