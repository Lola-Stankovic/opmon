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
  std::cout << "Setting a publisher!" << std::endl;
  if (metric_publish_ == nullptr) {
    metric_publish_ = makeMetricPublish(source, parameters);
  } else {
    ers::error(SetupPublisherError(ERS_HERE, "setupPublisher should be called once."));
  }
}

template <typename T> void 
MetricMonitor::registerMetric(const std::string& metricName, std::reference_wrapper<T> myMetric)
{
  MetricRegistry::getInstance().registerMetric<T>(metricName,  myMetric);
}

void
MetricMonitor::unregisterMetric(const std::string& metricName)
{  
  MetricRegistry::getInstance().unregisterMetric(metricName);
}

template <typename T> void
MetricMonitor::getValueOfMetric(const std::string& metricName)
{
  MetricRegistry::getInstance().getValueOfMetric<T>(metricName);
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
      // cout<< (*itr->second).getTypeName()<<'\n';
      metric_value = getValue(itr->second);
      std::cout<< "Metric name:" << metric_name << "\n";
      std::cout<< "Metric value:" << metric_value << "\n";
      metric_publish_->publishMetric(metric_name, application_name_, host_name_,  metric_value);
  }
}

double
MetricMonitor::publishThread()
{
  double exec_time = 0; 
  std::cout << "Going to sleep 1s...\n";
  std::this_thread::sleep_for(1s); 
  try {  
    auto start = std::chrono::high_resolution_clock::now();        
    std::cout << "The thread published the metrics " << "\n";
  
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
    std::cout << "Creating new thread \n" ;
    publishMetrics(metrics);
    //threads_.emplace_back(&MetricMonitor::publishThread, this);
  }

  for (auto& thread : threads_) {
    thread.join();
  }

  std::cout<<"### Finished monitoring \n" ;
  std::cout << "\n================ Statistics ================\n";
  std::cout << "Number of threads: " << number_of_threads_ << "\n" ; 

  /*
    for (auto &f: threads_) {
      auto s = f.get();
       std::cout << "Execution time [ms]: " << s << "\n";  
    }
  */

  std::cout << "\n============================================\n";
}                                                                    
