#ifndef METRIC_MONITOR_H_
#define METRIC_MONITOR_H_

#include <iostream>
#include <string>
#include <map>
#include <atomic>
#include <functional>
#include <any>
#include <thread>
#include <set>
#include <chrono>

#include "Issues.hpp"
#include "MetricRegistry.hpp"
#include "MetricPublish.hpp"

namespace dunedaq::opmonlib {

class MetricMonitor 
{
public:
  MetricMonitor(int rate, int numThreads,
                const std::string& hostName, const std::string& appName);
  ~MetricMonitor();
  void setupPublisher(const std::string& source, const std::map<std::string, std::string>& parameters);
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

  // Internals
  int rate_;
  long unsigned int number_of_threads_;
  std::string host_name_;
  std::string application_name_;

  std::vector<std::thread> threads_;
  
};

std::shared_ptr<MetricPublish>
makeMetricPublish(const std::string& source, const std::map<std::string, std::string>& par)
{
  std::string plugin_name = source + "MetricPublish";
  static cet::BasicPluginFactory bpf("duneMetricPublish", "make"); 
  std::shared_ptr<MetricPublish> cf_ptr;
  try {
    cf_ptr = bpf.makePlugin<std::shared_ptr<MetricPublish>>(plugin_name, par); 
  } catch (const cet::exception &cexpt) {
    throw MetricPublishCreationFailed(ERS_HERE, source, cexpt);
  }
    return cf_ptr;   
}

}

#endif /* METRIC_MONITOR_H_ */
