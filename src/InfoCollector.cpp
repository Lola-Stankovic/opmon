#include <iostream>
#include <string>
#include <chrono>
#include "InfoCollector.hpp"

using namespace dunedaq::opmonlib;
using namespace src;

void add( I&& infoclass ){
 
  m_infos[infoclass.get_classname()] = nlohmann::json{
    {"time", std::chrono::system_clock::now()},
    {"data", infoclass}};

}

