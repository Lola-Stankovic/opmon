#include <iostream>
#include <string>
#include <chrono>
#include "opmonlib/InfoCollector.hpp"

using namespace dunedaq::opmonlib;
using namespace std;

template<typename I> void InfoCollector::add( I&& infoclass ){
 
  m_infos.push_back(nlohmann::json{
    {"time", std::chrono::system_clock::now()},
    {"data", infoclass}});

}

