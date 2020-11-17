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
  void monitor(std::map<std::string, std::shared_ptr<MetricRefInterface>> metrics);
  void publishMetrics(std::map<std::string, std::shared_ptr<MetricRefInterface>> metrics);
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

std::shared_ptr<MetricPublish>
makeMetricPublish(const std::string& source, std::map<std::string, std::string> par)
{
  std::string plugin_name = source + "MetricPublish";
  static cet::BasicPluginFactory bpf("duneMetricPublish", "make"); 
  std::shared_ptr<MetricPublish> cf_ptr;
  try {
    cf_ptr = bpf.makePlugin<std::shared_ptr<MetricPublish>>(plugin_name, par); 
  } catch (const cet::exception &cexpt) {
     std::cout << "Catch exception." << std::endl;
    //throw CommandFacilityCreationFailed(ERS_HERE, uri, cexpt);
  }
    return cf_ptr;
    
}

#endif /* METRIC_MONITOR_H_ */





