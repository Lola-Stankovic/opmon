#include <iostream>
#include <string>
#include <nlohmann/json.hpp>

#include "opmonlib/OpmonService.hpp"
#include <cetlib/BasicPluginFactor.h>

using namespace dunedaq::opmonlib;

class stdoutOpmonService : public OpmonService
{
  public: 
    explicit stdoutOpmonService(std::string service) : OpmonService(service) {
      // Allocate resources as needed
      auto col = uri.find_last_of(':');
      auto sep = uri.find("://");
      std::string fname;
      if (col == std::string::npos || sep == std::string::npos) { // assume filename
	fname = uri;
      } else {
	fname = uri.substr(sep+3);
      }
    }

    void publish( nlohmann::json j )
    {
      std::cout << "Dumping json..." << std::endl;
      std::cout << j.dump(2) << std::endl;
    }

  protected:
    typedef OpmonService inherited;

};

extern "C" {
  std::shared_ptr<dunedaq::opmonlib::OpmonService> make(std::string service) {
    return std::shared_ptr<dunedaq::opmonlib::OpmonService>(new stdoutOpmonService(service));
  }
}

