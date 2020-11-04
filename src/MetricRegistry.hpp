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

#include "MetricInterface.hpp"

class MetricRegistry  {

private:
  MetricRegistry(){}
public:
  static MetricRegistry& getInstance(){
    static MetricRegistry theInstance;
    return theInstance;
  }

  size_t count() const;
  
  //mutable std::shared_mutex metrics_mutex_; /**< mutex that protects both MetricSets and metric_names. */
  typedef std::map<std::string, MetricPtr> MetricSet;
  MetricSet metric_set;
  typedef std::set<std::string> StringSet;
  StringSet metric_names_;

  template <typename T>
  void registerMetric(const std::string& metricName, 	std::reference_wrapper<T> myMetric);
    
  void unregisterMetric(const std::string& metricName);
    
  template <typename T>
  void getValueOfMetric(const std::string& metricName);

  std::map<std::string, MetricPtr> getMetrics();  
};





#endif /* METRIC_REGISTRY_H_ */





