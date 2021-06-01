/**
 * @file InfoCollector.hpp
 *
 * This is part of the DUNE DAQ Application Framework, copyright 2020.
 * Licensing/copyright details are in the COPYING file that you should have
 * received with this code.
 */

#ifndef OPMONLIB_INCLUDE_OPMONLIB_INFOCOLLECTOR_HPP_
#define OPMONLIB_INCLUDE_OPMONLIB_INFOCOLLECTOR_HPP_

#include <ctime>
#include <iostream>
#include <nlohmann/json.hpp>
#include <string>

namespace dunedaq::opmonlib {

class InfoCollector
{

public:
  // Templated method to grab info blocks
  template<typename I>
  void add(I&& infoclass)
  {
    nlohmann::json j;
    j["time"] = std::time(nullptr);
    j["data"] = infoclass;
    m_infos[infoclass.class_name] = j;
  }

  // Puny getter
  const nlohmann::json& get_collected_infos() { return m_infos; }

  // Method to construct hierarchical info
  void add(std::string name, InfoCollector& ic) { m_infos[name] = ic.get_collected_infos(); }
  // Method to check it there is any info stored
  bool is_empty() { return m_infos.empty(); }

private:
  nlohmann::json m_infos;
};

} // namespace dunedaq::opmonlib

#endif // OPMONLIB_INCLUDE_OPMONLIB_INFOCOLLECTOR_HPP_
