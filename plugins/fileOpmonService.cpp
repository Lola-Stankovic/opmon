/**
 * @file fileOpmonService.cpp
 *
 * This is part of the DUNE DAQ Application Framework, copyright 2020.
 * Licensing/copyright details are in the COPYING file that you should have
 * received with this code.
 */

#include "opmonlib/OpmonService.hpp"

#include <nlohmann/json.hpp>

#include <fstream>
#include <memory>
#include <string>

namespace dunedaq {

ERS_DECLARE_ISSUE(opmonlib, BadFile, "Can not open file to store opmon data: " << filename, ((std::string)filename))

} // namespace dunedaq

namespace dunedaq::opmonlib {

class fileOpmonService : public OpmonService
{
public:
  explicit fileOpmonService(std::string uri)
    : OpmonService(uri)
  {
    auto col = uri.find_last_of(':');
    auto sep = uri.find("://");
    std::string fname;
    if (col == std::string::npos || sep == std::string::npos) { // assume filename
      fname = uri;
    } else {
      fname = uri.substr(sep + 3);
    }

    m_ofs.open(fname, std::ios::out | std::ios::app);
    if (!m_ofs.is_open()) {
      ers::error(BadFile(ERS_HERE, fname));
    }
  }

  void publish(nlohmann::json j)
  {
    if (m_ofs.is_open()) {
      m_ofs << j.dump() << std::endl << std::flush;
    } else {
      TLOG() << "Opmon file is not open";
    }
  }

protected:
  typedef OpmonService inherited;

private:
  std::ofstream m_ofs;
};

} // namespace dunedaq::opmonlib

extern "C"
{
  std::shared_ptr<dunedaq::opmonlib::OpmonService> make(std::string service)
  {
    return std::shared_ptr<dunedaq::opmonlib::OpmonService>(new dunedaq::opmonlib::fileOpmonService(service));
  }
}
