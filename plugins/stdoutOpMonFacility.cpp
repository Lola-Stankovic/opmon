/**
 * @file stdoutOpMonFacility.cpp
 *
 * This is part of the DUNE DAQ Application Framework, copyright 2020.
 * Licensing/copyright details are in the COPYING file that you should have
 * received with this code.
 */

#include "opmonlib/OpMonFacility.hpp"


#include <iostream>
#include <memory>
#include <string>

namespace dunedaq::opmonlib {

class stdoutOpMonFacility : public OpMonServiceFacility
{
public:
  explicit stdoutOpMonFacility(std::string uri)
    : OpmonService(uri)
  {
    auto sep = uri.find("://");
    m_style = "flat";
    if (sep != std::string::npos) { // assume filename
      m_style = uri.substr(sep + 3);
    }
  }
  
  void publish(opmon::OpMonEntry && e) override 
  {

    static auto opt = stdoutOpMonFacility::generate_json_option();
  
    std::string json;
    google::protobuf::util::MessageToJsonString( e, & json, opt );
    std::cout << json << std::endl;   // MR: should we push to TLOG() instead?
    
  }

protected:
  static auto generate_json_option() {
    using namespace google::protobuf::util;
    static JsonPrintOptions opt;
    opt.add_whitespace = true;
    opt.preserve_proto_field_names = true;
    return opt;
  }

};

} // namespace dunedaq::opmonlib

DEFINE_DUNE_OPMON_FACILITY(dunedaq::opmonlib::stdoutOpMonFacility)
