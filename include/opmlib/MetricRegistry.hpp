#ifndef METRIC_REGISTRY_H_
#define METRIC_REGISTRY_H_

#include <iostream>
#include <string>
#include <map>
#include <atomic>
#include <functional>
#include <any>
#include <mutex>  
#include <shared_mutex>
#include <thread>
#include <set>

#include "OpmTypes.hpp"

using namespace dunedaq::opmlib;

class MetricRegistry  
{
private:
  MetricRegistry(){}

public:
  static MetricRegistry& getInstance(){
    static MetricRegistry theInstance;
    return theInstance;
  }

  size_t count() const;
  typedef std::map<std::string, MetricPtr> MetricSet;
  typedef std::set<std::string> StringSet;
  MetricSet metric_set;
  StringSet metric_names_;

  void unregisterMetric(const std::string& metricName);
  std::map<std::string, MetricPtr> getMetrics();
  
  template <typename T>
  void getValueOfMetric(const std::string& metricName) {
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

  template <typename T>
  void registerMetric(const std::string& metricName, std::reference_wrapper<T> myMetric) {
    MetricPtr new_metric = std::shared_ptr<MetricRefInterface>(new MetricRef<T> (myMetric));
    StringSet::iterator s_itt(metric_names_.find(metricName));
    if (s_itt == metric_names_.end()) {
      metric_names_.insert(metricName);
      metric_set.insert(std::make_pair(metricName, new_metric)).second;
    } else throw std::invalid_argument(
      metricName + " already exists as a different metric.");
  }
 
};

#endif /* METRIC_REGISTRY_H_ */





