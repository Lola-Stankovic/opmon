#include <iostream>
#include <string>
#include <nlohmann/json.hpp>

#include "opmonlib/OpmonService.hpp"

namespace dunedaq::opmonlib {
  
  class stdoutOpmonService : public OpmonService
  {
  public: 
    explicit stdoutOpmonService(std::string uri) : OpmonService(uri) {
    }

    void publish( nlohmann::json j )
    {
      std::cout << j.dump() << std::endl;
    }

  protected:
    typedef OpmonService inherited;

};

}

extern "C" {
  std::shared_ptr<dunedaq::opmonlib::OpmonService> make(std::string service) {
    return std::shared_ptr<dunedaq::opmonlib::OpmonService>(new dunedaq::opmonlib::stdoutOpmonService(service));
  }
}

