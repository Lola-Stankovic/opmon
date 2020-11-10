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

  template <typename T>
  void registerMetric(const std::string& metricName, std::reference_wrapper<T> myMetric);
  void unregisterMetric(const std::string& metricName);    
  template <typename T>
  void getValueOfMetric(const std::string& metricName);
  std::map<std::string, MetricPtr> getMetrics();
  
};

#endif /* METRIC_REGISTRY_H_ */





