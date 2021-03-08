#include <fstream>
#include <string>
#include <nlohmann/json.hpp>

#include "opmonlib/OpmonService.hpp"
#include <cetlib/BasicPluginFactor.h>

namespace duendaq {

     ERS_DECLARE_ISSUE(opmonlib, BadFile,
                     "Can not open file to store opmon data: " << filename,
                      ((std::string)filename))

}


using namespace dunedaq::opmonlib;

class fileOpmonService : public OpmonService
{
  public: 
    explicit fileOpmonService(std::string uri) : OpmonService(uri) {
        auto col = uri.find_last_of(':');
        auto sep = uri.find("://");
        std::string fname;
        if (col == std::string::npos || sep == std::string::npos) { // assume filename
          fname = uri;
        } else {
          fname = uri.substr(sep+3);
        } 

        m_ofs.open(fname, std::fstream::out);
        if (!ofs.is_open()) {
           ers::error(BadFile(ERS_HERE, fname);
        } 
    }

    void publish( nlohmann::json j )
    {
        if(m_ofs.is_open()) {  
           m_ofs << dump();
        }
    }
  protected:
    typedef OpmonService inherited;
  private:
    std::ofstream m_ofs;
};

extern "C" {
  std::shared_ptr<dunedaq::opmonlib::OpmonService> make(std::string service) {
    return std::shared_ptr<dunedaq::opmonlib::OpmonService>(new fileOpmonService(service));
  }
}

