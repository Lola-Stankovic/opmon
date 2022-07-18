/**
 * @file opmonservice.cpp
 *
 * This is part of the DUNE DAQ Software Suite, copyright 2022.
 * Licensing/copyright details are in the COPYING file that you should have
 * received with this code.
 */


#include "opmonlib/OpmonService.hpp"

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>


namespace py = pybind11;

namespace dunedaq::opmonlib {
namespace python {

void register_opmonservice(py::module& m)
{

  py::class_<opmonlib::OpmonService>(m,"OpmonService") 
      .def("publish", &opmonlib::OpmonService::publish);


  // Non-class function to create service. 
  m.def("makeOpmonService", &opmonlib::makeOpmonService, py::arg("service")); 

} 


} // namespace python
} // namespace dunedaq::opmonlib
