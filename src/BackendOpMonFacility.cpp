
#include "opmonlib/BackendOpMonFacility.hpp"

using namespace dunedaq::opmonlib;

void BackendOpMonFacility::publish(opmon::OpMonEntry && e) const {

  std::lock_guard<std::mutex> lk(m_mutex);
  m_list.push_back(e);
  
}


BackendOpMonFacility::data_t BackendOpMonFacility::get_entries() {

  std::lock_guard<std::mutex> lk(m_mutex);
  data_t ret;
  m_list.swap(ret);
  return ret;
}
