 #include<iostream>
#include<pybind11/pybind11.h>

namespace py = pybind11;

class Person
{

public:
    std::string habbit;             //兴趣爱好

private:
    std::string name;               //姓名
    std::string gender;             //性别
    int age;                        //年龄
    std::string idCardNum;          //身份证号码
    std::string address;            //住址

public:
    void setName(std::string name) {
        this->name = name;
    }

    void setAge(int age) {
        this->age = age;
    }

    void setGender(std::string gender) {
        this->gender = gender;
    }

    void setAddress(std::string address) {
        this->address = address;
    }

    void setIdCardNum(std::string idCardNum) {
        this->idCardNum = idCardNum;
    }

    std::string getName() {
        return this->name;
    }

    std::string getGender() {
        return this->gender;
    }

    int getAge() {
        return this->age;
    }

    std::string getAddress() {
        return this->address;
    }

    std::string getIdCardNum() {
        return this->idCardNum;
    }


public:
    //
    static std::string university;
    static std::string getUniversity() {
        return university;
    }

public:
    Person() {};
    Person(std::string name, std::string gender, int age) {
        this->name = name;
        this->gender = gender;
        this->age = age;
    }
    ~Person() {};

};

std::string Person::university = "xidian University";


PYBIND11_MODULE(demo3, m) {

    m.doc() = "Simple Class";
    
    py::class_<Person>(m, "Person")
        .def(py::init())
        .def(py::init<std::string, std::string, int>())
        .def("setName", &Person::setName)
        .def("setAge", &Person::setAge)
        .def("setGender", &Person::setGender)
        .def("setAddress", &Person::setAddress)
        .def("getName", &Person::getName)
        .def("getAge", &Person::getAge)
        .def("getGender", &Person::getGender)
        .def("getAddress", &Person::getAddress)
        .def_static("getUniversity", &Person::getUniversity)
        .def_readonly_static("university", &Person::university)
        .def_readwrite("habbit", &Person::habbit)
        .def_property("name", &Person::getName, &Person::setName)
        .def_property("age", &Person::getAge, &Person::setAge)
        .def_property("gender", &Person::getGender, &Person::setGender)
        .def_property("address", &Person::getAddress, &Person::setAddress)
        .def_property("idCardNum", &Person::getIdCardNum, &Person::setIdCardNum);


}

