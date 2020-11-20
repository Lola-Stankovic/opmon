#include <iostream>
#include <string>
#include <map>
#include <atomic>
#include <functional>
#include <any>
#include <mutex>  // For std::unique_lock
#include <shared_mutex>
#include <thread>
#include <future>
#include <set>
#include <chrono>

#include "opmonlib/MetricRegistry.hpp"
#include "opmonlib/MetricMonitor.hpp"

using namespace dunedaq::opmonlib;
using namespace std;

MetricMonitor::MetricMonitor(int rate, int numThreads,
                             const std::string& hostName, const std::string& appName)
  : rate_(rate)
  , number_of_threads_(numThreads)
  , host_name_(hostName)
  , application_name_(appName)
{
  should_run_ = true;
}

MetricMonitor::~MetricMonitor()
{
}

void
MetricMonitor::setupPublisher(const std::string& source, 
                              const std::map<std::string, std::string>& parameters)
{
  if (metric_publish_ == nullptr) {
    metric_publish_ = makeMetricPublish(source, parameters);
  } else {
    ers::error(SetupPublisherError(ERS_HERE, "setupPublisher should be called once."));
  }
}

double
MetricMonitor::getValue(std::shared_ptr<MetricRefInterface> ref)
{
  double retval = 0;
  if ((*ref).getTypeName() == typeid(std::atomic<float>).name()) {
    std::reference_wrapper<std::atomic<float>> value = 
      dynamic_cast<MetricRef<std::atomic<float>>&>(*ref).getValue(); 
    retval = (double)value.get();
  } else if ((*ref).getTypeName() == typeid(std::atomic<int>).name()) { 
    std::reference_wrapper<std::atomic<int>> value =
      dynamic_cast<MetricRef<std::atomic<int>>&>(*ref).getValue(); 
    retval = (double)value.get();
  } else if ((*ref).getTypeName() == typeid(std::atomic<double>).name()) { 
    std::reference_wrapper<std::atomic<double>> value =
      dynamic_cast<MetricRef<std::atomic<double>>&>(*ref).getValue(); 
    retval = (double)value.get();
  } else if ((*ref).getTypeName() == typeid(std::atomic<size_t>).name()) {
    std::reference_wrapper<std::atomic<size_t>> value =
      dynamic_cast<MetricRef<std::atomic<size_t>>&>(*ref).getValue(); 
    retval = (double)value.get();
  }
  return retval;
}

void
MetricMonitor::publishMetrics(std::map<std::string, std::shared_ptr<MetricRefInterface>> metrics)
{ 
  for (auto itr = metrics.begin(), itr_end = metrics.end(); itr != itr_end; ++itr) {
    std::string metric_name = itr->first;
    double metric_value = 0;
  
    metric_value = getValue(itr->second);
    ERS_INFO("Metric name:" << metric_name);
    ERS_INFO("Metric value:" << metric_value);
    metric_publish_->publishMetric(metric_name, application_name_, host_name_,  metric_value);
  }
}

double
MetricMonitor::publishThread()
{
  double exec_time = 0; 
  ERS_INFO("Going to sleep 1s...");
  std::this_thread::sleep_for(1s); 
  try {  
    auto start = std::chrono::high_resolution_clock::now();        
    publishMetrics(MetricRegistry::getInstance().getMetrics());
    auto end = std::chrono::high_resolution_clock::now();
    exec_time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
  } catch (const std::exception& e) {
    ers::error(PublishingMetricsError(ERS_HERE, "Error while publishing metrics."));
  }
  return exec_time;
}

void
MetricMonitor::stop() 
{
  should_run_ = false;
}

void
MetricMonitor::monitor(std::map<std::string, std::shared_ptr<MetricRefInterface>> metrics) 
{
  for (uint64_t j=0; j<number_of_threads_; j++) {
    ERS_INFO("Creating new thread:");
    publishMetrics(metrics);
    //threads_.emplace_back(&MetricMonitor::publishThread, this);
  }

  for (auto& thread : threads_) {
    thread.join();
  }

  ERS_INFO("### Finished monitoring");
  ERS_INFO("================ Statistics ================");
  ERS_INFO("Number of threads: " << number_of_threads_ ); 

  /*
    for (auto &f: threads_) {
      auto s = f.get();
      ERS_INFO("Execution time [ms]: " << s << "\n");  
    }
  */

  ERS_INFO("============================================");
}                                                                    
