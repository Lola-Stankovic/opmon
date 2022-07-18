/**
 * @file infocollecter.cpp
 *
 * This is part of the DUNE DAQ Software Suite, copyright 2022.
 * Licensing/copyright details are in the COPYING file that you should have
 * received with this code.
 */


#include "opmonlib/InfoCollector.hpp"

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>


namespace py = pybind11;

//namespace python {
namespace dunedaq::opmonlib {

void register_infocollector(py::module& m)
{

  py::class_<opmonlib::InfoCollector>(m,"InfoCollector") 
      .def("get_collected_infos", &opmonlib::InfoCollector::get_collected_infos)
      // Temporarily comment these ones out while I figure out how to hand template method
      .def("add", &opmonlib::InfoCollector::add, py::arg("infoclass"))
      .def("add", static_cast<void (InfoCollector::*)(std::string, InfoCollector&)>(&InfoCollector::add), "Explicit arguments")
      .def("is_empty", &opmonlib::InfoCollector::is_empty); 

} 


} // namespace dunedaq::opmonlib
//} // namespace python
