/**
 * @file MonitorableObject.cpp
 *
 * This is part of the DUNE DAQ Application Framework, copyright 2020.
 * Licensing/copyright details are in the COPYING file that you should have
 * received with this code.
 */

#include <opmonlib/MonitorableObject.hpp>
#include <opmonlib/Utils.hpp>

#include <google/protobuf/util/time_util.h>

using namespace dunedaq::opmonlib;

void MonitorableObject::register_child( std::string name, new_child_ptr p ) {

  std::unique_lock<std::mutex> lock(m_children_mutex);
  
  m_children.push_back( p );

  p -> m_opmon_name = name;
  p -> inherit_parent_properties( *this );

}


bool MonitorableObject::publish( google::protobuf::Message && m ) const {

  auto timestamp = google::protobuf::util::TimeUtil::GetCurrentTime();

  auto e = to_entry( m );

  if ( e.data().empty() ) {
    return false;
  }
  
  e.set_opmon_id( get_opmon_id() );
  *e.mutable_time() = timestamp;

  return m_service->publish(e);
 
}


void MonitorableObject::collect( opmon_level ) {

  generate_opmon_data();

  std::unique_lock<std::mutex> lock(m_children_mutex);

  for ( auto c : m_children ) {

    auto ptr = c.lock();
    if ( ptr ) {
      ptr -> collect(); // MR can we make this async? there is not point to wait all are done
    }
    else {
      // we should remove this as the pointer became empty
    }
  }
  
}


