#ifndef METRIC_MONITOR_H_
#define METRIC_MONITOR_H_

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
#include <chrono>

#include "MetricRegistry.hpp"
#include "MetricPublish.hpp"

class MetricMonitor 
{
public:
  MetricMonitor(const std::string& appName, const std::string& hostName,
                const long unsigned int numThreads, int rateLimiter);
  ~MetricMonitor();
  void setupPublisher(const std::string& source, std::map<std::string, std::string> parameters);
  void start();    
  void stop();
  double publishThread();
  void monitor();
  void publishMetrics(std::map<std::string, std::shared_ptr<MetricRefInterface>> metrics);
  void ccmPublishMetrics(const std::string& metricName, const std::string& application_name,
                         const std::string& host_name,double metric_value);
  double getValue(std::shared_ptr<MetricRefInterface> ref);
    
  template <typename T>
  void registerMetric(const std::string& metricName, std::reference_wrapper<T> myMetric);
  void unregisterMetric(const std::string& metricName);
  template <typename T>
  void getValueOfMetric(const std::string& metricName);
   
private:
  std::atomic<bool> should_run_; // or active_
  std::shared_ptr<MetricPublish> metric_publish_;
  int rate_;
  long unsigned int number_of_threads_;
  std::string host_name_;
  std::string application_name_;
  std::vector<std::thread> threads_;
  //mutable std::shared_mutex metrics_mutex_; 
  
};

#endif /* METRIC_MONITOR_H_ */





