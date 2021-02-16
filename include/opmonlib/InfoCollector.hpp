#ifndef OPMONLIB_INCLUDE_OPMONLIB_INFOCOLLECTOR_HPP_
#define OPMONLIB_INCLUDE_OPMONLIB_INFOCOLLECTOR_HPP_

#include <nlohmann/json.hpp>
#include <iostream>
#include <chrono>

namespace dunedaq::opmonlib {

  class InfoCollector {

    public:

      // Templated method to grab info blocks
      template<typename I>
      void add( I&& infoclass );

      // Puny getter
      const nlohmann::json& get_collected_infos() { return m_infos; }

    private:
      nlohmann::json m_infos;


  };

}

#endif
