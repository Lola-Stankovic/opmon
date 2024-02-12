/**
 * @file schema_conversion_test.cxx Test application that tests and demonstrates
 * the conversion from a generic protobuf message to an opmon entry.
 *
 * This is part of the DUNE DAQ Application Framework, copyright 2020.
 * Licensing/copyright details are in the COPYING file that you should have
 * received with this code.
 */

#include "opmonlib/Utils.hpp"
#include "opmonlib/info/test.pb.h"

#define BOOST_TEST_MODULE schema_conversion_test // NOLINT



#include "boost/test/unit_test.hpp"

#include <google/protobuf/util/json_util.h>

using namespace dunedaq::opmonlib;

BOOST_AUTO_TEST_SUITE(Schema_Conversion_Test)

BOOST_AUTO_TEST_CASE(conversion) {

  const int64_t int_value = 394;
  const float float_value = 3.14;
  const double double_value = 6.28;
  const std::string string_value = "my_test" ;
  const bool bool_value = true;
  
  dunedaq::opmon::TestInfo ti;
  ti.set_int_example( int_value );
  ti.set_float_example( double_value );
  ti.set_string_example( string_value );
  ti.set_bool_example( bool_value );

  dunedaq::opmon::ComplexInfo ci;
  ci.set_another_float(float_value);
  *ci.mutable_sub_message() = ti;
  //ci.mutable_r_field() -> Add(42);   // this gives linking problems

  auto test_entry = to_entry( ti );
  auto complex_entry = to_entry( ci );

  std::string res;
  google::protobuf::util::MessageToJsonString( test_entry, & res );
  std::cout << res << std::endl;

  res.clear();
  google::protobuf::util::MessageToJsonString( complex_entry, & res );
  std::cout << res << std::endl;

  BOOST_TEST( test_entry.data().size() == 4 );   //check that all the entry of the simple schema are in
  BOOST_TEST( complex_entry.data().size() == 1 );   //check that complex objects are not serialised

  //auto value = test_entry.data().find("int_example");  // this causes linking problems
  //  auto final_int =  value->second.int8_value();
  //BOOST_TEST( final_int == int_value );
  
  // for ( const auto & e : c.get_entries() ) {

  
  // }
}


BOOST_AUTO_TEST_SUITE_END()

