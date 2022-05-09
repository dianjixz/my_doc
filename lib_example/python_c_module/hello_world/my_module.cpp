#define PY_SSIZE_T_CLEAN 
#include<Python.h>



typedef struct image {
    int w;
    int h;
    int bpp;
    uint8_t *data;
}image_t;




// 没错，这就够了，什么stdio.h就都有了
static PyObject* say_hello(PyObject* self, PyObject* args) {
    printf("Hello world, I just a demo.\r\n");
    image_t mk;
	mk.w = 10;
	printf("nihao%d\r\n",mk.w);




    return PyLong_FromLong(0);
}
// PyMethodDef 是一个结构体
static PyMethodDef my_methods[] = {
    { "say", say_hello,METH_VARARGS , "Just show a greeting." },
    {NULL, NULL, 0, NULL}
};

static struct PyModuleDef mymodule = {
    PyModuleDef_HEAD_INIT,
    "mymodule",
    NULL,
    -1,
    my_methods
};
PyMODINIT_FUNC PyInit_mymodule(void) {
    return PyModule_Create(&mymodule);
}
