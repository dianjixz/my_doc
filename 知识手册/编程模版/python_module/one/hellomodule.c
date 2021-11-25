#define PY_SSIZE_T_CLEAN
#include <Python.h>


static PyObject *
say_hello_ak(PyObject *self, PyObject *args)
{
	const char * greeting = "hello\n";
	int sts;

	sts = printf("%s", greeting);
	return PyLong_FromLong(sts);
}

static PyMethodDef helloMethods[] = {
	{"say", say_hello_ak,  METH_VARARGS, "say hello to frank ak .."},
	{NULL, NULL, 0, NULL}	
};

PyDoc_STRVAR(hello_doc, "this module is for someone who want to deep learn Python.\n\n ");

static struct PyModuleDef hellomodule = {
	PyModuleDef_HEAD_INIT,
	"hello",
	hello_doc,
	-1,
	helloMethods
};

PyMODINIT_FUNC
PyInit_hello(void)
{
	return PyModule_Create(&hellomodule);
}

