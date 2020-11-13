#include <iostream>
#include <string>
#include <map>
#include <mutex>
#include <shared_mutex>
#include <thread>
#include <unistd.h>
#include <future>
#include <set>
#include <chrono>
#include <cstdint>
#include <stdio.h>

#include "opmlib/MetricPublish.hpp"

using namespace dunedaq::opmlib;
using namespace std::chrono_literals;

uint64_t
timeSinceEpochMillisec()
{
  using namespace std::chrono;
  return duration_cast<nanoseconds>(system_clock::now().time_since_epoch()).count();
}

void
MetricPublish::publishMetricByHTTPRequest(const std::string& metricName, const std::string& application_name,
                                          const std::string& host_name,double metric_value)
{
/* CURL *curl;
  CURLcode res;
 
  curl_global_init(CURL_GLOBAL_ALL);
  std::string uri="http://"+ getInfluxDbUrl()+ ":" + std::to_string(getPort()) + 
    "/write?db="+getDatabaseName();

  std::string data_binary=metricName+",host="+host_name+",application="+application_name+" value =
    "+std::to_string(metric_value)+" "+ std::to_string(timeSinceEpochMillisec());
  std::cout<<uri<< '\n';
    
  curl = curl_easy_init();
  if(curl) {
    curl_easy_setopt(curl, CURLOPT_URL, uri.c_str());
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data_binary.c_str());

    res = curl_easy_perform(curl);
    if(res != CURLE_OK)
      fprintf(stderr, "curl_easy_perform() failed: %s\n",
        curl_easy_strerror(res));
    curl_easy_cleanup(curl);
  }
  curl_global_cleanup();
*/
} 
   
void
MetricPublish::ccmPublishMetric(const std::string& metricName, const std::string& application_name,
                                const std::string& host_name,double metric_value)
{
/*  
  dune::daq::ccm::CtrlNode node;
  std::string uri="/write?db="+getDatabaseName();
  std::string data_binary=metricName+",host="+host_name+","+"application="+application_name + 
    " value="+std::to_string(metric_value);
 
  try {
    const uint16_t port1 = ccm::Valid::portNumber(getPort());
    std::cout << "Port number: " << getPort() << '\n';
    std::cout << "Database name: " << getDatabaseName() << '\n';
    node.addClient("client1", "localhost", port1); 
  } catch (std::exception e) {
    std::cout << "Could not add controlled/controller entity: " << e.what() << '\n';
  } 

  std::cout << "Going to sleep 2s...\n";
  std::this_thread::sleep_for(2s);   
  std::string content = uri + data_binary;

  try {
    node.sendCommand("client1", data_binary);
  } catch (std::exception e) {
    std::cout << "Couldn't send command to  controller client: " << e.what() << '\n';
  }

  node.teardown();
*/
}

