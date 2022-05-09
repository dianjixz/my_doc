//文件名：func_wrapper.cpp
#include <pybind11/pybind11.h>  
#include "whjy.h"  
  
namespace py = pybind11;  
  
PYBIND11_MODULE(nihao, m){  
    m.doc() = "simple example";  
  
    py::class_<student>(m, "student")  
        .def(py::init<string>())  
        .def("setName", &student::setName)  
        .def_readonly("name", &student::name)  
        .def_readwrite("Chinese", &student::Chinese)  
        .def_readwrite("Mathematics", &student::Mathematics)  
        .def_readwrite("English", &student::English)  
        .def_readwrite("total", &student::total);  
    m.def("calc", &calc);  
}
