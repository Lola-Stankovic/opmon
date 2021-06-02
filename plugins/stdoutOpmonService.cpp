/**
 * @file stdoutOpmonService.cpp
 *
 * This is part of the DUNE DAQ Application Framework, copyright 2020.
 * Licensing/copyright details are in the COPYING file that you should have
 * received with this code.
 */

#include "opmonlib/OpmonService.hpp"

#include <nlohmann/json.hpp>

#include <iostream>
#include <memory>
#include <string>

namespace dunedaq::opmonlib {

class stdoutOpmonService : public OpmonService
{
public:
  explicit stdoutOpmonService(std::string uri)
    : OpmonService(uri)
  {
    auto sep = uri.find("://");
    m_style = "flat";
    if (sep != std::string::npos) { // assume filename
      m_style = uri.substr(sep + 3);
    }
  }

  void publish(nlohmann::json j)
  {
    if (m_style == "flat") {
      std::cout << std::setw(4) << j.flatten() << '\n'; // NOLINT(runtime/output_format)
    } else if (m_style == "formatted") {
      std::cout << j.dump(2) << std::endl; // NOLINT(runtime/output_format)
    } else {
      std::cout << j.dump() << std::endl; // NOLINT(runtime/output_format)
    }
  }

protected:
  typedef OpmonService inherited;

private:
  std::string m_style;
};

} // namespace dunedaq::opmonlib

extern "C"
{
  std::shared_ptr<dunedaq::opmonlib::OpmonService> make(std::string service)
  {
    return std::shared_ptr<dunedaq::opmonlib::OpmonService>(new dunedaq::opmonlib::stdoutOpmonService(service));
  }
}
