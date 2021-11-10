/**
 * @file InfoCollector.hpp
 *
 * This is part of the DUNE DAQ Application Framework, copyright 2020.
 * Licensing/copyright details are in the COPYING file that you should have
 * received with this code.
 */

#ifndef OPMONLIB_INCLUDE_OPMONLIB_INFOCOLLECTOR_HPP_
#define OPMONLIB_INCLUDE_OPMONLIB_INFOCOLLECTOR_HPP_

#include <nlohmann/json.hpp>

#include <ctime>
#include <iostream>
#include <string>
#include "opmonlib/JSONTags.hpp"


namespace dunedaq::opmonlib {

class InfoCollector
{

public:

  // Templated method to grab info blocks
  template<typename I>
  void add(I&& infoclass)
  {
    nlohmann::json j_infoblock;
    j_infoblock[JSONTags::time] = std::time(nullptr);
    j_infoblock[JSONTags::data] = infoclass;

    m_infos[JSONTags::properties][infoclass.info_type] = j_infoblock;
  }

  // Puny getter
  const nlohmann::json& get_collected_infos() { return m_infos; }

  // Method to construct hierarchical info
  void add(std::string name, InfoCollector& ic) { m_infos[JSONTags::children][name] = ic.get_collected_infos(); }
  // Method to check it there is any info stored
  bool is_empty() { return m_infos.empty(); }

private:
  nlohmann::json m_infos;
};

} // namespace dunedaq::opmonlib

#endif // OPMONLIB_INCLUDE_OPMONLIB_INFOCOLLECTOR_HPP_
