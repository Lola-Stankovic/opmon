/**
 * @file moniotorable_object_test.cxx Test application that tests MonitorableObject.
 *
 * This is part of the DUNE DAQ Application Framework, copyright 2020.
 * Licensing/copyright details are in the COPYING file that you should have
 * received with this code.
 */

#include "opmonlib/info/test.pb.h"
#include "opmonlib/MonitorableObject.hpp"
#include "opmonlib/OpMonManager.hpp"

#define BOOST_TEST_MODULE monitorable_object_test // NOLINT

#include "boost/test/unit_test.hpp"

#include <vector>

using namespace dunedaq::opmonlib;
using namespace dunedaq::opmon;

BOOST_AUTO_TEST_SUITE(Monitorable_Object_Test)

struct my_fixture {

  class TestManager : public OpMonManager {
  public :
    using OpMonManager::collect;
    TestManager( std::string session,
		 std::string name )
      : OpMonManager(session, name) {;}
  };
  
  class TestObject : public MonitorableObject {

  public:
    using MonitorableObject::register_child;
    using MonitorableObject::publish;
    TestObject() : MonitorableObject() {;}
  };

  my_fixture()
    : manager("test", "manager")
    , node_p(new TestObject) {;}  
  
  TestManager manager;
  std::shared_ptr<TestObject> node_p;
   
};


BOOST_FIXTURE_TEST_CASE( opmon_ids, my_fixture ) {

  BOOST_CHECK_EQUAL( to_string(node_p->get_opmon_id()), "" );
  BOOST_CHECK_EQUAL( to_string(manager.get_opmon_id()), "test.manager" );

  manager.register_child("child", node_p);
  BOOST_CHECK_EQUAL( to_string(node_p->get_opmon_id()), "test.manager.child" );
}


BOOST_FIXTURE_TEST_CASE( opmon_level, my_fixture ) {

  manager.register_child("child", node_p);
  auto parent_level = manager.get_opmon_level();
  auto child_level  = node_p->get_opmon_level();

  BOOST_CHECK_EQUAL( child_level, parent_level );
  BOOST_CHECK_EQUAL( child_level, to_level(SystemOpMonLevel::kAll) );

  
  manager.set_opmon_level( to_level(SystemOpMonLevel::kDisabled) );

  parent_level = manager.get_opmon_level();
  child_level  = node_p->get_opmon_level();
  BOOST_CHECK_EQUAL( child_level, parent_level );
  BOOST_CHECK_EQUAL( child_level, to_level(SystemOpMonLevel::kDisabled) );
}


BOOST_FIXTURE_TEST_CASE( counters, my_fixture ) {

  manager.register_child("child", node_p);

  std::shared_ptr<TestObject> child(new TestObject);
  node_p->register_child("grand_child", child);
  
  dunedaq::opmon::TestInfo ct;
  ct.set_string_example( "child_test" );
  ct.set_int_example( 1000 );

  node_p->publish( dunedaq::opmon::TestInfo(ct) );

  ct.set_int_example( 2000 );
  manager.set_opmon_level(to_level(SystemOpMonLevel::kDisabled));
  BOOST_CHECK_NO_THROW( child->publish( dunedaq::opmon::TestInfo(ct) ) );
  
  auto data = manager.collect() ;
  BOOST_CHECK_EQUAL( data.n_published_measurements(), 1 );
  BOOST_CHECK_EQUAL( data.n_ignored_measurements(), 1 );
  BOOST_CHECK_EQUAL( data.n_errors(), 0 );
}






BOOST_AUTO_TEST_SUITE_END()
