#ifndef OPMONLIB_INCLUDE_OPMONLIB_INFOMANAGER_HPP_
#define OPMONLIB_INCLUDE_OPMONLIB_INFOMANAGER_HPP_

#include <nlohmann/json.hpp>
#include "opmonlib/InfoProvider.hpp"
#include <atomic>
#include <thread>

namespace dunedaq::opmonlib {

  class InfoManager 
  {
    public:
      InfoManager( std::string service ); // Constructor, std::string is placeholder for instancing each info service
      void publish_info( int level );
      nlohmann::json gather_info( int level);
      void set_provider( opmonlib::InfoProvider& p );
    
    private:
      void start(uint32_t interval_sec, uint32_t level);
      void run(uint32_t interval_sec, uint32_t level);
      void stop();
 
      mutable opmonlib::InfoProvider *m_ip = nullptr;
      std::string m_service;
      std::atomic<bool> m_running; 
      std::thread m_thread;

  };

}

#endif
