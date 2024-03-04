/**
 * @file MonitorableObject.cpp
 *
 * This is part of the DUNE DAQ Application Framework, copyright 2020.
 * Licensing/copyright details are in the COPYING file that you should have
 * received with this code.
 */

#include <opmonlib/MonitorableObject.hpp>
#include <opmonlib/Utils.hpp>
#include <logging/Logging.hpp>

#include <google/protobuf/util/time_util.h>

using namespace dunedaq::opmonlib;

void MonitorableObject::register_child( std::string name, new_child_ptr p ) {

  std::unique_lock<std::mutex> lock(m_children_mutex);

  // check if the name is already present to ensure uniqueness
  auto it = m_children.find(name) ;
  if ( it != m_children.end() ) {
    // This not desired because names are suppposed to be unique
    // But if the pointer is expired, there is no harm in override it
    if ( it -> second.expired() ) {
      ers::warning(NonUniqueChildName(ERS_HERE, name));
    }
    else {
      throw NonUniqueChildName(ERS_HERE, name);
    }
  }
  
  m_children[name] = p;

  p -> m_opmon_name = name;
  p -> inherit_parent_properties( *this );

}


void MonitorableObject::publish( google::protobuf::Message && m ) const noexcept {

  auto timestamp = google::protobuf::util::TimeUtil::GetCurrentTime();

  auto e = to_entry( m );

  if ( e.data().empty() ) {
    ers::warning( EntryWithNoData(ERS_HERE, e.measurement() ) );
    return ;
  }
  
  e.set_opmon_id( get_opmon_id() );
  *e.mutable_time() = timestamp;

  // this pointer is always garanteed to be filled, even if with a null Facility.
  // But the facility can fail
  try {
    m_facility->publish(std::move(e));
  } catch ( OpMonPublishFailure e ) {
    ers::error(e);
  }
 
}


void MonitorableObject::collect( opmon_level l) {

  generate_opmon_data();

  std::unique_lock<std::mutex> lock(m_children_mutex);

  for ( auto it = m_children.begin(); it != m_children.end(); ) {

    auto ptr = it->second.lock();
    
    if( ptr ) {
      ptr->collect(l);  // MR: can we make this an async? There is no point to wait all done here
    }

    // prune the dead links
    if ( it->second.expired() ) {
      it = m_children.erase(it);
    } else {
      ++it;
    }
  }

}


void MonitorableObject::inherit_parent_properties( const MonitorableObject & parent ) {

  
}


