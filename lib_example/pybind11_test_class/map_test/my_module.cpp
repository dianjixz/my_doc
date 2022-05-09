
extern "C"{
    #define PY_SSIZE_T_CLEAN 
     #include<Python.h> 
     #include"conversion.h"
     
     } 
#include<pybind11/stl.h>
#include<vector>
#include<list>
#include<array>
#include<map>
#include<set>


using namespace std;


namespace py = pybind11;


// struct image {
//     int w;
//     int h;
//     int bpp;
//     union {
//         uint8_t *pixels;
//         uint8_t *data;
//     };
// }



class img_data
{
    private:

    protected:
        struct image arg_img;
    
    public:
        
        img_data();
        img_data(const py::bytes &bytes,int w,int h);
        ~img_data();
        void python_send_to_c_image(const py::bytes &bytes,int w,int h)
        {


            
            // std::string value = static_cast<std::string>(bytes);

            // uint8_t *nihao;
            // nihao = (uint8_t *)value.data();



            // struct image img_data;
            // img_data.h = h;
            // img_data.w = w;
            // img_data.data = (uint8_t*)malloc(w * h * 3);
            // memcpy(img_data.data,nihao,w * h * 3);

            // this->arg_img.data = (uint8_t*)malloc(w * h * 2);
            // rgb888_to_rgb565_img(&img_data , &this->arg_img);
            // free(img_data.data);
            // printf("nihao----------------------------------------------------------------\r\n");




        }
        py::bytes img_back_rgb888();

};


img_data :: img_data()
{
    this->arg_img.data = NULL;
}
img_data :: img_data(const py::bytes &bytes,int w,int h)
{

    std::string value = static_cast<std::string>(bytes);

    uint8_t *nihao;
    nihao = (uint8_t *)value.data();



    struct image img_data;
    img_data.h = h;
    img_data.w = w;
    img_data.data = (uint8_t*)malloc(w * h * 3);
    memcpy(img_data.data,nihao,w * h * 3);

    this->arg_img.data = (uint8_t*)malloc(w * h * 2);
    rgb888_to_rgb565_img(&img_data , &this->arg_img);
    free(img_data.data);

}



// void img_data :: python_send_to_c_image(const py::bytes &bytes,int w,int h)
// {

    // std::string value = static_cast<std::string>(bytes);

    // uint8_t *nihao;
    // nihao = (uint8_t *)value.data();



    // struct image img_data;
    // img_data.h = h;
    // img_data.w = w;
    // img_data.data = (uint8_t*)malloc(w * h * 3);
    // memcpy(img_data.data,nihao,w * h * 3);

    // this->arg_img.data = (uint8_t*)malloc(w * h * 2);
    // rgb888_to_rgb565_img(&img_data , &this->arg_img);
    // free(img_data.data);
    // printf("nihao----------------------------------------------------------------\r\n");
// }


// py::bytes return_bytes() {
//     const char *data = "\x01\x00\x02\x00";
//     return std::string(data, 4);
// }




// std::string print_bytes(const py::bytes &bytes) {
//     std::string ret = "bytes[";
//     const auto value = static_cast<std::string>(bytes);
//     for (size_t i = 0; i < value.length(); ++i) {
//         ret += std::to_string(static_cast<int>(value[i])) + " ";
//     }
//     ret.back() = ']';
//     return ret;
// }


py::bytes img_data :: img_back_rgb888()
{

    if(this->arg_img.data == NULL) return NULL;


    struct image img_data;
    img_data.data = (uint8_t*)malloc(arg_img.w * arg_img.h * 3);

 
    rgb565_to_rgb888_img(&this->arg_img,&img_data);

    std::string data;
    void *nihao;
    nihao = img_data.data;


    // data = 
    // free(img_data.data);

    return std::string((char*)nihao,img_data.h * img_data.w * 3);;
}





img_data :: ~img_data()
{
    free(this->arg_img.data);
}



PYBIND11_MODULE(nihao, m) {

    m.doc() = "This is a simple demo using C++ STL";
    
    pybind11::class_<img_data>(m, "img_data" )
    .def(pybind11::init())
    .def(pybind11::init<const py::bytes,int ,int>())
    .def("send_to_c_image",&img_data::python_send_to_c_image)
    .def("img_back_rgb888",&img_data::img_back_rgb888);
}






// typedef list<PyObject*> LISTSTR;

// vector<LISTSTR> li_set(vector<float> &a) {

//     vector<LISTSTR> out;
//     for (int i = 0; i < a.size(); i++) {
//         out.push_back(Py_BuildValue("i", a[i]));
//     }

//     return out;
// }


// py::bytes return_bytes() {
//     const char *data = "abcd";
//     return std::string(data, 4);
// }

// void print_bytes(const py::bytes &bytes) {


//     std::string value = static_cast<std::string>(bytes);

//     char *nihao;
//     nihao = (char *)value.data();


    
//     printf("bytes:-----%x\r\n",nihao[0]);
//     printf("bytes:-----%x\r\n",nihao[1]);
//     printf("bytes:-----%x\r\n",nihao[2]);
//     printf("bytes:-----%x\r\n",nihao[3]);
//     printf("bytes:-----%x\r\n",nihao[4]);
//     printf("bytes:-----%x\r\n",nihao[5]);


//     // std::string ret = "bytes[";
//     // const auto value = static_cast<std::string>(bytes);
    
//     // for (size_t i = 0; i < value.length(); ++i) {
//     //     ret += std::to_string(static_cast<int>(value[i])) + " ";
//     // }
//     // ret.back() = ']';
//     // return ret;
// }


// PYBIND11_MODULE(nihao, m) {

//     m.doc() = "This is a simple demo using C++ STL";
//     m.def("return_bytes",&return_bytes);
//     m.def("print_bytes",&print_bytes);
// }




// vector<float> vec_add(vector<float>& a, vector<float>& b) {

//     vector<float> out;
//     assert(a.size() == b.size());
//     for (int i = 0; i < a.size(); i++)
//     {
//         out.push_back(a[i] + b[i]);
//     }

//     return out;

// }



// array<float, 20> vec_sin(array<float, 20>& x) {
//     array<float, 20> out;
//     for (int i = 0; i < x.size(); i++)
//     {
//         out[i] = sinf(i);
//     }
//     return out;
// }

// map<string, int> get_map(vector<string>& keys, vector<int> values) {
     
//     map<string, int> out;
    
//     for (int i = 0; i < keys.size(); i++)
//     {
//         out.insert({ keys[i], values[i] });
//     }

//     return out;
// }

// set<string> get_set(vector<string>& values) {

//     set<string> out;
//     for (auto& i:values)
//     {
//         out.insert(i);
//     }

//     return out;
// }








// #define PY_SSIZE_T_CLEAN 
// #include<Python.h>

// #include <pybind11/pybind11.h>  



// #include <iostream>
// #include<string>
// #include<list>


// namespace py = pybind11;  

// using namespace std;

// typedef list<PyObject> LISTSTR;





// LISTSTR test;
// // PyObject *nihao1;
// // PyObject *nihao1;
// // PyObject *nihao1;



// test.push_back(Py_BuildValue("i", 15));                               //back
// test.push_front(Py_BuildValue("f", 1.5));                       //middle back
// test.push_front(Py_BuildValue("s", "nihao"));                           //front  middle back




  
// PYBIND11_MODULE(nihao, m){  
//     m.doc() = "simple example";  
  
//     py::class_<LISTSTR>(m, "clist")  





//         .def(py::init<string>())  
//         .def("setName", &student::setName)  
//         .def_readonly("name", &student::name)  
//         .def_readwrite("Chinese", &student::Chinese)  
//         .def_readwrite("Mathematics", &student::Mathematics)  
//         .def_readwrite("English", &student::English)  
//         .def_readwrite("total", &student::total);  
//     m.def("calc", &calc);  
// }

















    // cout<<test.front()<<endl;                        //front
    // cout<<*test.begin()<<endl;                       //front

    // cout<<test.back()<<endl;                        //back
    // cout<<*(test.rbegin())<<endl;                //back

    // test.pop_front();                                   //middle back
    // test.pop_back();                                    //middle

    // cout<<test.front()<<endl;                    //middle




// // 没错，这就够了，什么stdio.h就都有了
// static PyObject* say_hello(PyObject* self, PyObject* args) {
//     printf("Hello world, I just a demo.");
//     return PyLong_FromLong(0);
// }
// // PyMethodDef 是一个结构体
// static PyMethodDef my_methods[] = {
//     { "say", say_hello,METH_VARARGS , "Just show a greeting." },
//     {NULL, NULL, 0, NULL}
// };

// static struct PyModuleDef mymodule = {
//     PyModuleDef_HEAD_INIT,
//     "mymodule",
//     NULL,
//     -1,
//     my_methods
// };
// PyMODINIT_FUNC PyInit_mymodule(void) {
//     return PyModule_Create(&mymodule);
// }
