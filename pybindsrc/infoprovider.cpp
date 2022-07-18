/**
 * @file infoprovider.cpp
 *
 * This is part of the DUNE DAQ Software Suite, copyright 2022.
 * Licensing/copyright details are in the COPYING file that you should have
 * received with this code.
 */


#include "opmonlib/InfoProvider.hpp"
#include "opmonlib/InfoCollector.hpp"

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>


namespace py = pybind11;

namespace dunedaq::opmonlib {
namespace python {

void register_infoprovider(py::module& m)
{

  py::class_<opmonlib::InfoProvider>(m,"InfoProvider") 
      // methods
      .def("gather_stats", &opmonlib::InfoProvider::gather_stats, py::arg("ic"), py::arg("level"));

} 


} // namespace python
} // namespace dunedaq::opmonlib
