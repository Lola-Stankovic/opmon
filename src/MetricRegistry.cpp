#include <iostream>
#include <string>
#include <map>
#include <atomic>
#include <functional>
#include <any>
#include <mutex>  // For std::unique_lock
#include <shared_mutex>
#include <thread>
#include <set>

#include "opmonlib/MetricRegistry.hpp"

using namespace dunedaq::opmonlib;

size_t
MetricRegistry::count() const
{
  //std::shared_lock<std::shared_mutex> read_lock(metrics_mutex_);
  return metric_names_.size();
}

void
MetricRegistry::unregisterMetric(const std::string& metricName)
{
  //std::unique_lock<std::shared_mutex> wlock(metrics_mutex_);
  StringSet::iterator s_itt(metric_names_.find(metricName));   
  if (s_itt != metric_names_.end()) {
    metric_set.erase(metricName);
    metric_names_.erase(s_itt);
  } else {
    ers::error(MetricRegistryError(ERS_HERE, metricName + " doesn't exist."));
    }
}

std::map<std::string, std::shared_ptr<MetricRefInterface>>
MetricRegistry::getMetrics() 
{
  std::map<std::string, std::shared_ptr<MetricRefInterface>> ret_set;
  for (std::map<std::string, std::shared_ptr<MetricRefInterface>>::iterator itr =
    metric_set.begin(), itr_end = metric_set.end(); itr != itr_end; ++itr) {
      //std:: cout << itr->second->getTypeName()<< '\n';    
      ret_set[itr->first] = std::static_pointer_cast<MetricRefInterface>(itr->second);
  }
  return ret_set;
}                                                                    
