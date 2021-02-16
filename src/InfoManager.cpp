#include <iostream>
#include <string>

#include "opmonlib/InfoManager.hpp"
#include "opmonlib/InfoCollector.hpp"

using namespace dunedaq::opmonlib;
using namespace std;


InfoManager::InfoManager( std::string service ) {
  m_service = service;
}


void InfoManager::publish_info( int level ) {

  // Use m_mod_mgr and m_time_interval_sec here?
  // add? 

  nlohmann::json j = gather_info(level);
  
  std::cout << "Something Interesting: " << j.dump() << std::endl;  // print json?

}


nlohmann::json InfoManager::gather_info( int level ) {

  nlohmann::json j;
  
  for ( auto& [n, m] : m_modulemap) {   // How we gather stats in DAQModuleManager not sure
 
    dunedaq::opmonlib::InfoCollector ic;
    m->m_ip.gather_stats(ic, level);       
    j[m->get_name()] = ic.get_collected_infos();
 
  }

  return j;

}


void InfoManager::set_provider( const opmonlib::InfoProvider &p ){

  // Set the data member to point to selected InfoProvider
  m_ip = &p;

}


