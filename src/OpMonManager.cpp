/**
 * @file OpMonManager.cpp
 *
 * This is part of the DUNE DAQ Application Framework, copyright 2020.
 * Licensing/copyright details are in the COPYING file that you should have
 * received with this code.
 */

#include <opmonlib/OpMonManager.hpp>

using namespace dunedaq::opmonlib;


OpMonManager::OpMonManager( std::string session,
			    std::string name,
			    std::string opmon_facility_uri)
  : m_parent_opmon_id(session)
  , m_opmon_name(name) {

  m_facility = makeOpMonFacility(opmon_facility_uri);
  
}


OpMonManager::~OpMonManager() {

  stop();
}


void OpMonManager::start(std::chrono::seconds interval, opmon_level level) {

  auto name = get_opmon_id();
  name += ".opmon";
  
  m_thread = jthread( & OpMonManager::run, this, interval, level );
  auto handle = m_thread.native_handle;
  auto rc =  pthread_setname_np(handle, name.c_str());    
   if (rc != 0) {
    ers::warning(ThreadNameTooLong(ERS_HERE, name));
  }
  
}



void OpMonManager::stop() {
  m_thread.request_stop();
  m_thread.join();
}
  


void OpMonManager::run(std::chrono::seconds interval, opmon_level level ) {

  auto sleep_interval = std::chrono::milliseconds(100);
    
  auto stop_token = m_thead.get_stop_token();
  decltype(stop_token) counter = 0;
  
  
  while( ! stop_token.stop_requested() ) {

    std::this_thread::sleep_for(sleep_interval);
    counter += sleep_interval ;

    if ( counter > interval ) {
      counter = 0;
      try {
	collect(level);
      } catch ( ers::Issue i ) {
	ers::error( ErrorWhileCollecting(ERS_HERE, i) );
      } catch (std::exception e ) {
	ers::error( ErrorWhileCollecting(ERS_HERE, e) );
      } catch (...) {
	ers::error( ErrorWhileCollecting(ERS_HERE) );
      }
    }
  }
}
