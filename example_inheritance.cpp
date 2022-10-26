#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <stdio.h>

namespace py = pybind11;
using pymod = pybind11::module;


class Pet {
public:
    Pet(const std::string &name) : name(name) { }
    void setName(const std::string &name_) { name = name_; }
    const std::string &getName() const { return name; }
private:
    std::string name;
};


class Dog : public Pet {
  public:
    Dog(const std::string &name) : Pet(name) { }
    void setName(const std::string &name_) { name = name_; }
    const std::string &getName() const { return name; }
    std::string bark() const { return "woof!"; }
  private:
    std::string name;
};


PYBIND11_MODULE(example_inheritance, mmod)
{
  constexpr auto MODULE_DESCRIPTION = "Just testing out mpi with python.";
  mmod.doc() = MODULE_DESCRIPTION;

  py::class_<Pet>(mmod, "Pet")
    .def(py::init<const std::string &>())
    .def("setName", &Pet::setName)
    .def("getName", &Pet::getName);

  py::class_<Dog, Pet>(mmod, "Dog")
    .def(py::init<const std::string &>())
    .def("bark", &Dog::bark);
  
}
