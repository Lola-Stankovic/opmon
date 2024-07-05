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

#include <chrono>

using namespace dunedaq::opmonlib;

void MonitorableObject::register_child( std::string name, new_child_ptr p ) {

  std::lock_guard<std::mutex> lock(m_children_mutex);

  // check if the name is already present to ensure uniqueness
  auto it = m_children.find(name) ;
  if ( it != m_children.end() ) {
    // This not desired because names are suppposed to be unique
    // But if the pointer is expired, there is no harm in override it
    if ( it -> second.expired() ) {
      ers::warning(NonUniqueChildName(ERS_HERE, name, to_string(get_opmon_id())));
    }
    else {
      throw NonUniqueChildName(ERS_HERE, name, to_string(get_opmon_id()));
    }
  }
  
  m_children[name] = p;

  p -> m_opmon_name = name;
  p -> inherit_parent_properties( *this );

  TLOG() << "Child " << name << " registered to " << to_string(get_opmon_id()) ;
}


void MonitorableObject::publish( google::protobuf::Message && m,
				 CustomOrigin && co,
				 OpMonLevel l,
				 const element_id & element ) const noexcept {

  auto timestamp = google::protobuf::util::TimeUtil::GetCurrentTime();

  if ( ! MonitorableObject::publishable_metric( l, get_opmon_level() ) ) {
    ++m_ignored_counter;
    return;
  }
  
  auto e = to_entry( m, co );

  if ( e.data().empty() ) {
    ers::warning( EntryWithNoData(ERS_HERE, e.measurement() ) );
    return ;
  }

  auto id = get_opmon_id() ;
  if ( ! element.empty() ) {
    if ( m_children.count( element ) > 0 ) {
      ers::error(NonUniqueChildName(ERS_HERE, element,
				    to_string(get_opmon_id())));
      ++m_error_counter;
      return;
    }
    else {
      id = id + element;
    }
  }
  
  *e.mutable_origin() = id;
  *e.mutable_time() = timestamp;

  // this pointer is always garanteed to be filled, even if with a null Facility.
  // But the facility can fail
  try {
    m_facility->publish(std::move(e));
    ++m_published_counter;
  } catch ( const OpMonPublishFailure & e ) {
    ers::error(e);
    ++m_error_counter;
  }
 
}


opmon::MonitoringTreeInfo MonitorableObject::collect() noexcept {

  auto start_time = std::chrono::high_resolution_clock::now();

  TLOG() << "Collecting data from " << to_string(get_opmon_id());
  
  opmon::MonitoringTreeInfo info;

  info.set_n_invalid_links(0);

  try {
    generate_opmon_data();
  } catch ( const ers::Issue & i ) {
    ++m_error_counter;
    auto cause_ptr = i.cause();
    while ( cause_ptr ) {
      ++m_error_counter;
      cause_ptr = cause_ptr->cause();
    }
    ers::error( ErrorWhileCollecting(ERS_HERE, to_string(get_opmon_id()), i) );
  } catch (  const std::exception & e ) {
    ++m_error_counter;
    ers::error( ErrorWhileCollecting(ERS_HERE, to_string(get_opmon_id()), e) );
  } catch (...) {
    ++m_error_counter;
    ers::error( ErrorWhileCollecting(ERS_HERE, to_string(get_opmon_id())) );
  }

  info.set_n_published_measurements( m_published_counter.exchange(0) );
  info.set_n_ignored_measurements( m_ignored_counter.exchange(0) );
  info.set_n_errors( m_error_counter.exchange(0) );
  if (info.n_published_measurements() > 0) {
    info.set_n_publishing_nodes(1);
  } 

  std::lock_guard<std::mutex> lock(m_children_mutex);

  info.set_n_registered_nodes( m_children.size() );

  unsigned int n_invalid_links = 0;
  
  for ( auto it = m_children.begin(); it != m_children.end(); ) {

    auto ptr = it->second.lock();
    
    if( ptr ) {
      auto child_info = ptr->collect();  // MR: can we make this an async? There is no point to wait all done here
      info.set_n_registered_nodes( info.n_registered_nodes() + child_info.n_registered_nodes() );
      info.set_n_publishing_nodes( info.n_publishing_nodes() + child_info.n_publishing_nodes() );
      info.set_n_invalid_links( info.n_invalid_links() + child_info.n_invalid_links() );
      info.set_n_published_measurements( info.n_published_measurements() + child_info.n_published_measurements() );
      info.set_n_ignored_measurements( info.n_ignored_measurements() + child_info.n_ignored_measurements() );
      info.set_n_errors( info.n_errors() + child_info.n_errors() );
    }

    // prune the dead links
    if ( it->second.expired() ) {
      it = m_children.erase(it);
      ++n_invalid_links;
    } else {
      ++it;
    }
  }

  info.set_n_invalid_links( info.n_invalid_links() + n_invalid_links );
  
  
  auto stop_time = std::chrono::high_resolution_clock::now();

  auto duration = std::chrono::duration_cast<std::chrono::microseconds>( stop_time - start_time );
  info.set_clockwall_elapsed_time_us( duration.count() );
  info.set_cpu_elapsed_time_us( duration.count() ); // MR: this is ok for now but needs changing if we use async collection
  
  return info;
}


void MonitorableObject::set_opmon_level( OpMonLevel l ) noexcept {

  m_opmon_level = l;

  std::lock_guard<std::mutex> lock(m_children_mutex);
  for ( const auto & [key,wp] : m_children ) {
    auto p = wp.lock();
    if (p) {
      p->set_opmon_level(l);
    }
  }
}

void MonitorableObject::inherit_parent_properties( const MonitorableObject & parent ) {

  m_facility = parent.m_facility;
  m_parent_id = parent.get_opmon_id();
  m_opmon_level = parent.get_opmon_level();
  
  std::lock_guard<std::mutex> lock(m_children_mutex);

  for ( const auto & [key,wp] : m_children ) {

    auto p = wp.lock();
    if ( p ) {
      p->inherit_parent_properties(*this);
    }
    
  }
  
}

