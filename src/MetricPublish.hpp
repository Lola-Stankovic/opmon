#ifndef METRIC_PUBLISH_H_
#define METRIC_PUBLISH_H_

#include <iostream>
#include <string>
#include <map>
#include <atomic>
#include <functional>
#include <memory>
#include <any>
#include <mutex>  
#include <shared_mutex>
#include <thread>
#include <set>

#include <cetlib/BasicPluginFactory.h>
#include <cetlib/compiler_macros.h>

class MetricPublish
{
public:	 
  void publishMetric(const std::string& metricName, const std::string& application_name,
                     const std::string& host_name,double metric_value);
  void publishMetricByHTTPRequest(const std::string& metricName, const std::string& application_name,
                                  const std::string& host_name,double metric_value);
  void ccmPublishMetric(const std::string& metricName, const std::string& application_name,
                        const std::string& host_name,double metric_value);
    
};

std::shared_ptr<MetricPublish>
makeMetricPublish(std::string const& source, std::map<std::string, std::string> parameters)
{
    //influxdb or file
    std::string plugin_name = source + "MetricPublish";
    static cet::BasicPluginFactory bpf("duneMetricPublish", "make"); 
    //std::string uri = "localhost:8086
    return bpf.makePlugin<std::shared_ptr<MetricPublish>>(plugin_name, parameters);
}

#endif /* METRIC_PUBLISH_H_ */





