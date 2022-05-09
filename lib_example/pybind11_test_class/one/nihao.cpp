extern "C"{
    #define PY_SSIZE_T_CLEAN 
     #include<Python.h> 
     #include"conversion.h"
     
     } 
#include <iostream>
#include <pybind11/pybind11.h>


#include<vector>
#include<list>
#include<array>
#include<map>
#include<set>


namespace py = pybind11;

class IMG_DATA {

protected:
    struct image arg_img;

public:
    IMG_DATA(void);
    IMG_DATA(const py::bytes &bytes,int w,int h);
    ~IMG_DATA();

    void python_send_to_c_image(const py::bytes &bytes,int w,int h);
    py::bytes img_back_rgb888();
    py::str py_str();
    py::str py_repr();
    py::object buildmk_test();

private:
};


IMG_DATA :: IMG_DATA(void) 
{
    this->arg_img.data = NULL;
}


IMG_DATA :: IMG_DATA(const py::bytes &bytes,int w,int h) 
{
    std::string value = static_cast<std::string>(bytes);
    uint8_t *nihao;
    nihao = (uint8_t *) value.data();

    struct image img_data;
    img_data.h = h;
    img_data.w = w;
    img_data.data = (uint8_t*)malloc(w * h * 3);
    memcpy(img_data.data,nihao,w * h * 3);

    this->arg_img.data = (uint8_t*)malloc(w * h * 2);
    rgb888_to_rgb565_img(&img_data , &this->arg_img);
    free(img_data.data);
}


IMG_DATA :: ~IMG_DATA() 
{
    free(this->arg_img.data);
    std::cout << "我被删除了!" <<std::endl;
}

void IMG_DATA :: python_send_to_c_image(const py::bytes &bytes,int w,int h) {

    std::string value = static_cast<std::string>(bytes);
    uint8_t *nihao;
    nihao = (uint8_t *) value.data();

    struct image img_data;
    img_data.h = h;
    img_data.w = w;
    img_data.data = (uint8_t*)malloc(w * h * 3);
    memcpy(img_data.data,nihao,w * h * 3);

    this->arg_img.data = (uint8_t*)malloc(w * h * 2);
    rgb888_to_rgb565_img(&img_data , &this->arg_img);
    free(img_data.data);
}

py::bytes IMG_DATA :: img_back_rgb888()
{

    if(this->arg_img.data == NULL) return NULL;

    struct image img_data;
    img_data.data = (uint8_t*)malloc(arg_img.w * arg_img.h * 3);
 
    rgb565_to_rgb888_img(&this->arg_img,&img_data);

    std::string data;
    void *nihao;
    nihao = img_data.data;

    data = std::string((char*)nihao,img_data.h * img_data.w * 3);
    free(img_data.data);

    return data;
}
py::str IMG_DATA :: py_str()
{
    py::str nihao = "this is test!\r\n";
    return nihao;

}
py::str IMG_DATA :: py_repr()
{
    py::str nihao = "this is test repr!\r\n";
    return nihao;

}
py::object IMG_DATA :: buildmk_test()
{
    auto py_o = py::module::import("nihao");
    auto mk = py_o.attr("mk_test");
    py::object o = mk();
    return o;
}

class mk_test
{
private:
    /* data */
public:
    mk_test()
    {

    }
    ~mk_test()
    {
        std::cout << "mk_test 遗言 我被删除了!" <<std::endl;
    }
    py::str py_str()
    {
        py::str  o =  "py_str mk_test build susscee!\r\n";
        return o;
    }
    py::str py_repr()
    {
        py::str  o =  "py_repr mk_test build susscee!\r\n";
        return o;
    }
};





PYBIND11_MODULE( nihao, m ){
    m.doc() = "pybind11 example";
    pybind11::class_<IMG_DATA>(m, "IMG_DATA" )
        .def( pybind11::init() )
        .def( pybind11::init<const py::bytes,int ,int >() )
        .def( "send_to_c_image", &IMG_DATA::python_send_to_c_image )
        .def( "img_back_rgb888", &IMG_DATA::img_back_rgb888 )
        .def("__str__",&IMG_DATA::py_str)
        .def("__repr__",&IMG_DATA::py_repr)
        .def("buildmk_test",&IMG_DATA::buildmk_test);
    pybind11::class_<mk_test>(m, "mk_test" )
        .def( pybind11::init() )
        .def("__str__",&mk_test::py_str)
        .def("__repr__",&mk_test::py_repr);


}

