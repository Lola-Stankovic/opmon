/**
 * @file stdoutOpMonFacility.cpp
 *
 * This is part of the DUNE DAQ Application Framework, copyright 2020.
 * Licensing/copyright details are in the COPYING file that you should have
 * received with this code.
 */

#include "opmonlib/OpMonFacility.hpp"
#include <google/protobuf/util/json_util.h>

#include <iostream>
#include <memory>
#include <string>

namespace dunedaq::opmonlib {

class stdoutOpMonFacility : public OpMonFacility
{
public:
  explicit stdoutOpMonFacility(std::string uri)
    : OpMonFacility(uri)
  {
    // auto sep = uri.find("://");
    // we'll keep this free for possible improvements

    m_opt.add_whitespace = true;
    m_opt.preserve_proto_field_names = true;

  }
  
  void publish(opmon::OpMonEntry && e) override {
    std::string json;
    google::protobuf::util::MessageToJsonString( e, & json, m_opt );
    std::cout << json << std::endl;   // MR: should we push to TLOG() instead?
  }

private:
  google::protobuf::util::JsonPrintOptions m_opt;
  
};

} // namespace dunedaq::opmonlib

DEFINE_DUNE_OPMON_FACILITY(dunedaq::opmonlib::stdoutOpMonFacility)
