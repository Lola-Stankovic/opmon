/**
 * @file OpMonContainer.hpp
 *
 * This is part of the DUNE DAQ Application Framework, copyright 2020.
 * Licensing/copyright details are in the COPYING file that you should have
 * received with this code.
 */

#ifndef OPMONLIB_INCLUDE_OPMONLIB_OPMONCONTAINER_HPP_
#define OPMONLIB_INCLUDE_OPMONLIB_OPMONCONTAINER_HPP_


#include <string>
#include <list>

#include <google/protobuf/message.h>
#include "opmonlib/opmon_entry.pb.h"

#include <google/protobuf/util/time_util.h>

namespace dunedaq::opmonlib {

class OpMonContainer
{
  std::list<dunedaq::opmon::OpMonEntry> m_entries;
  
public:

  const auto & get_entries() const { return m_entries; }

  void add(::google::protobuf::Message && m, std::string id="");
  void add(OpMonContainer &&, std::string id="");
  void add_parent_info(const std::string & parent);

  auto clear() {
    decltype(m_entries) ret;
    std::swap(m_entries, ret);
    return ret;
  }
};

} // namespace dunedaq::opmonlib

#endif // OPMONLIB_INCLUDE_OPMONLIB_OPMONCONTAINER_HPP_
