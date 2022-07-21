/**
 * @file opmoncore.cpp
 *
 * This is part of the DUNE DAQ Software Suite, copyright 2022.
 * Licensing/copyright details are in the COPYING file that you should have
 * received with this code.
 */


#include "opmonlib/InfoManager.hpp"
#include "opmonlib/OpmonService.hpp"
#include "opmonlib/InfoCollector.hpp"
#include "opmonlib/InfoProvider.hpp"


#include <pybind11/pybind11.h>
#include <pybind11/stl.h>


namespace py = pybind11;

namespace dunedaq::opmonlib {
namespace python {

void register_opmoncore(py::module& m)
{

  // opmonlib classes
  py::class_<opmonlib::OpmonService>(m,"OpmonService") 
      .def("publish", &opmonlib::OpmonService::publish);

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

  py::class_<opmonlib::InfoCollector>(m,"InfoCollector") 
      .def("get_collected_infos", &opmonlib::InfoCollector::get_collected_infos)
      // Temporarily comment these ones while I figure out how to handle them
      //.def("set", static_cast<void (Pet::*)(int)>(&Pet::set), "Set the pet's age")
      //.def("add", &opmonlib::InfoCollector::add, py::arg("infoclass"))
      .def("add", static_cast<void (InfoCollector::*)(std::string, InfoCollector&)>(&InfoCollector::add), "Explicit arguments")
      .def("is_empty", &opmonlib::InfoCollector::is_empty); 

  py::class_<opmonlib::InfoProvider>(m,"InfoProvider") 
      // methods
      .def("gather_stats", &opmonlib::InfoProvider::gather_stats, py::arg("ic"), py::arg("level"));

  // Non-class function to create service. 
  m.def("makeOpmonService", &opmonlib::makeOpmonService, py::arg("service")); 

} 


} // namespace python
} // namespace dunedaq::opmonlib
