#ifndef OPMONLIB_INCLUDE_OPMONLIB_BACKENDOPMONFACILITY_HPP_
#define OPMONLIB_INCLUDE_OPMONLIB_BACKENDOPMONFACILITY_HPP_

#include "opmonlib/OpMonFacility.hpp"
#include "opmonlib/Utils.hpp"

#include <list>
#include <mutex>

namespace dunedaq::opmonlib {

  class BackendOpMonFacility : public OpMonFacility {

    using data_t = std::list<opmon::OpMonEntry>;
    
  public:
    BackendOpMonFacility() : OpMonFacility("Backend://") {;}
    
    void publish(opmon::OpMonEntry && e) const override ;
    
    data_t get_entries() ;

  private:
    mutable data_t m_list;
    mutable std::mutex m_mutex;
  };
}

#endif //OPMONLIB_INCLUDE_OPMONLIB_BACKENDOPMONFACILITY_HPP_
