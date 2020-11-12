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

#include "opmlib/MetricRegistry.hpp"

size_t
MetricRegistry::count() const
{
  //std::shared_lock<std::shared_mutex> read_lock(metrics_mutex_);
  return metric_names_.size();
}

template <typename T> void
MetricRegistry::registerMetric(const std::string& metricName, std::reference_wrapper<T> myMetric)
{
  MetricPtr new_metric = std::shared_ptr<MetricRefInterface>(new MetricRef<T> (myMetric));
  StringSet::iterator s_itt(metric_names_.find(metricName));
  if (s_itt == metric_names_.end()) {
    metric_names_.insert(metricName);
    metric_set.insert(std::make_pair(metricName, new_metric)).second;
  } else throw std::invalid_argument(
    metricName + " already exists as a different metric.");
}

void
MetricRegistry::unregisterMetric(const std::string& metricName)
{
  //std::unique_lock<std::shared_mutex> wlock(metrics_mutex_);
  StringSet::iterator s_itt(metric_names_.find(metricName));   
  if (s_itt != metric_names_.end()) {
    metric_set.erase(metricName);
    metric_names_.erase(s_itt);
  } else throw std::invalid_argument(
    metricName + " doesn't exist.");
}

template <typename T> void
MetricRegistry::getValueOfMetric(const std::string& metricName)
{
  //std::unique_lock<std::shared_mutex> wlock(metrics_mutex_);
  StringSet::iterator s_itt(metric_names_.find(metricName));   
  if (s_itt != metric_names_.end()) {
    if (typeid(T).name() == typeid(std::atomic<float>).name()) {
      std::reference_wrapper<std::atomic<float>> value =
        dynamic_cast<MetricRef<std::atomic<float>>&>(*metric_set[metricName]).getValue(); 
      double a= (double) value.get();
    } else if(typeid(T).name() == typeid(std::atomic<int>).name()) {
        std::reference_wrapper<std::atomic<int>> value =
          dynamic_cast<MetricRef<std::atomic<int>>&>(*metric_set[metricName]).getValue(); 
      }
      std::reference_wrapper<T> value =
        dynamic_cast<MetricRef<T>&>(*metric_set[metricName]).getValue(); 
      std::cout<< value.get()<<'\n';
  } else throw std::invalid_argument(
    metricName + " doesn't exist.");

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
                                                                    
