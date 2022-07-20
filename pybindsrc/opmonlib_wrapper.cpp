/**
 * @file opmonlib_wrapper.cpp
 *
 * This is part of the DUNE DAQ Software Suite, copyright 2022.
 * Licensing/copyright details are in the COPYING file that you should have
 * received with this code.
 */

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;

namespace dunedaq::opmonlib {
namespace python {
	
	extern void register_opmoncore(py::module &);
	extern void register_opmonissues(py::module &);
	extern void register_opmontags(py::module &);

PYBIND11_MODULE(_daq_opmonlib_py, opmon_module) {

    opmon_module.doc() = "python bindings for opmonlib"; // optional module docstring

    // Is this necessary?
    py::module_ core_module = opmon_module.def_submodule("core");
    
    // register the components
    dunedaq::opmonlib::python::register_opmoncore(core_module);
    dunedaq::opmonlib::python::register_opmonissues(core_module);
    dunedaq::opmonlib::python::register_opmontags(core_module);    
}

} // namespace python
} // namespace dunedaq