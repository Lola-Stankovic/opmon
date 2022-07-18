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
	
	extern void register_infocollector(py::module &);
	extern void register_infomanager(py::module &);
	extern void register_infoprovider(py::module &);
	extern void register_issues(py::module &);
	extern void register_jsontag(py::module &);
	extern void register_opmonservice(py::module &);

PYBIND11_MODULE(_daq_opmonlib_py, opmon_module) {

    opmon_module.doc() = "python bindings for opmonlib"; // optional module docstring

    // Is this necessary?
    py::module_ core_module = opmon_module.def_submodule("core");
    
    // register the components
    dunedaq::opmonlib::python::register_infocollector(core_module);
    dunedaq::opmonlib::python::register_infomanager(core_module);
    dunedaq::opmonlib::python::register_infoprovider(core_module);
    dunedaq::opmonlib::python::register_issues(core_module);
    dunedaq::opmonlib::python::register_jsontag(core_module);
    dunedaq::opmonlib::python::register_opmonservice(core_module);
    
}

} // namespace python
} // namespace dunedaq