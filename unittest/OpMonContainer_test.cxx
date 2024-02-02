/**
 * @file OpMonContainer_test.cxx Test application that tests and demonstrates
 * the functionality of the OpMonContainer class.
 *
 * This is part of the DUNE DAQ Application Framework, copyright 2020.
 * Licensing/copyright details are in the COPYING file that you should have
 * received with this code.
 */

#include "opmonlib/OpMonContainer.hpp"
#include "opmonlib/info/test.pb.h"

#define BOOST_TEST_MODULE OpMonContainer_test // NOLINT



#include "boost/test/unit_test.hpp"

#include <google/protobuf/util/json_util.h>


using namespace dunedaq::opmonlib;

BOOST_AUTO_TEST_SUITE(OpMonContainer_Test)

BOOST_AUTO_TEST_CASE(conversion) {

  dunedaq::opmonlib::OpMonContainer c;

  dunedaq::opmon::TestInfo ti;
  ti.set_int_example( 394 );

  c.add(std::move(ti), "my_info");

  
  std::string res;
  google::protobuf::util::MessageToJsonString( *c.get_entries().begin(), & res );
  std::cout << res;
}


BOOST_AUTO_TEST_SUITE_END()

