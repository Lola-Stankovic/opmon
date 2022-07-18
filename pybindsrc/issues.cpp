/**
 * @file issues.cpp
 *
 * This is part of the DUNE DAQ Software Suite, copyright 2022.
 * Licensing/copyright details are in the COPYING file that you should have
 * received with this code.
 */


#include "opmonlib/Issues.hpp"

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>


namespace py = pybind11;

namespace dunedaq {
namespace python {

void
register_issues(py::module& m)
{

  // Wrap ERS_DECLARE_ISSUE
  // Can I get away with just giving what I think is the only user-specified argument (error)?
  m.def("ERS_DECLARE_ISSUE", &dunedaq::ERS_DECLARE_ISSUE, py::arg("error"));   
  // Or do I have to write something more complicated?

} 

} // namespace python
} // namespace dunedaq
