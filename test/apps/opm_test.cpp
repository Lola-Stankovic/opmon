#include <iostream>
#include <string>
#include <atomic>
#include <mutex>  // For std::unique_lock
#include <shared_mutex>
#include <chrono>
#include <future>
#include <thread>

#include "../src/MetricRegistry.hpp"
#include "../src/MetricMonitor.cpp"
#include "../src/MetricRegistry.cpp"
#include "../src/MetricMonitor.hpp"

int main(int argc, char** argv)
{
  std::atomic<float> myMetric(0.1);
  std::atomic<int> myMetric_int(5);
  std::atomic<double> myMetric_double(500);
  std::atomic<size_t> myMetric_sizet(40960);
   
  MetricMonitor mmonitor("localhost", "prometheus_lola", 8086,"AppName",  "HostName", 1 , 1);
  mmonitor.registerMetric<std::atomic<float>>("Temperature", std::ref(myMetric));   
  mmonitor.getValueOfMetric<std::atomic<float>>("Temperature");
  mmonitor.registerMetric<std::atomic<int>>("Humidity", std::ref(myMetric_int));   
  mmonitor.getValueOfMetric<std::atomic<int>>("Humidity");
  
  // 5 seconds later, I'll modify myMetrics:
  for (unsigned i=0; i < 10; ++i) {
    myMetric = myMetric + 1.1;
    myMetric_int=myMetric_int + 1;
    myMetric_double=myMetric_double + 1;
    myMetric_sizet=myMetric_sizet+1;
    //std::this_thread::sleep_for(5s);
  }

  mmonitor.getValueOfMetric<std::atomic<float>>("Temperature");
  mmonitor.getValueOfMetric<std::atomic<int>>("Humidity");
  mmonitor.monitor();
                                                                    
}
                                                                     
