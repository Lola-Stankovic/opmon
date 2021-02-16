#ifndef OPMONLIB_INCLUDE_OPMONLIB_INFOMANAGER_HPP_
#define OPMONLIB_INCLUDE_OPMONLIB_INFOMANAGER_HPP_

#include <iostream>
#include <nlohmann/json.hpp>
#include "opmonlib/InfoProvider.hpp"

namespace dunedaq::opmonlib {

  class InfoManager 
  {
    public:
      InfoManager( std::string service ); // Constructor, std::string is placeholder for instancing each info service
      void publish_info( int level );
      nlohmann::json gather_info( int level);
      void set_provider( opmonlib::InfoProvider &p );
    
    private:
      uint32_t m_time_interval_sec;
      //const DAQModuleManager& m_mod_mgr; No longer needed?
      opmonlib::InfoProvider* m_ip;
      std::string m_service;

  };

}

#endif
