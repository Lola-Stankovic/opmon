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
  ti.set_float_example( 3.14 );
  ti.set_string_example( "test" );
  ti.set_bool_example( true );

  c.add(std::move(ti), "my_info");

  dunedaq::opmon::ComplexInfo ci;
  ci.set_another_float(6.28);
  *ci.mutable_sub_message() = ti;
  //ci.mutable_r_field() -> Add(42);   // this gives linking problems

  c.add(std::move(ci), "complex");

  
  for ( const auto & e : c.get_entries() ) {
    std::string res;
    google::protobuf::util::MessageToJsonString( e, & res );
    std::cout << res << std::endl;
  }
}


BOOST_AUTO_TEST_SUITE_END()

