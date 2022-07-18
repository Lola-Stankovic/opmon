/**
 * @file infomanager.cpp
 *
 * This is part of the DUNE DAQ Software Suite, copyright 2022.
 * Licensing/copyright details are in the COPYING file that you should have
 * received with this code.
 */


#include "opmonlib/InfoManager.hpp"

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>


namespace py = pybind11;

namespace dunedaq::opmonlib {
namespace python {

void register_infomanager(py::module& m)
{

  py::class_<opmonlib::InfoManager>(m,"InfoManager") 
      // constructors 
      .def(py::init<std::string &>())
      .def(py::init<opmonlib::OpmonService &>())
      // methods
      .def("publish_info", &opmonlib::InfoManager::publish_info, py::arg("level"))
      .def("gather_info", &opmonlib::InfoManager::gather_info, py::arg("level"))
      .def("set_provider", &opmonlib::InfoManager::set_provider, py::arg("p"))
      .def("set_tags", &opmonlib::InfoManager::set_tags, py::arg("tags"))
      .def("start", &opmonlib::InfoManager::start, py::arg("interval_sec"), py::arg("level"))
      .def("stop", &opmonlib::InfoManager::stop);

} 


} // namespace python
} // namespace dunedaq::opmonlib
