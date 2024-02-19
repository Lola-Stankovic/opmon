/**
 * @file opmon_facility_test.cxx Test application that tests and demonstrates
 * the conversion from a generic protobuf message to an opmon entry.
 *
 * This is part of the DUNE DAQ Application Framework, copyright 2020.
 * Licensing/copyright details are in the COPYING file that you should have
 * received with this code.
 */

#inlcude "opmonlib/OpMonFacility.hpp"
#include "opmonlib/info/test.pb.h"

#define BOOST_TEST_MODULE opmon_facility_test // NOLINT

#include "boost/test/unit_test.hpp"

using namespace dunedaq::opmonlib;
using namespace dunedaq::opmon;

BOOST_AUTO_TEST_SUITE(Opmon_Facility_Test)


BOOST_AUTO_TEST_CASE(Invalid_Creation) {

}


BOOST_AUTO_TEST_CASE(STD_Cout_facility) {

}


BOOST_AUTO_TEST_CASE(File_facility) {

}


BOOST_AUTO_TEST_SUITE_END()

