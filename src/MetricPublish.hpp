#ifndef METRIC_PUBLISH_H_
#define METRIC_PUBLISH_H_

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

class MetricPublish
{
public:
  void setPublisher(int port, const std::string& database_name,
                    const std::string& influxdb_uri);	 
  void publishMetric(const std::string& metricName, const std::string& application_name,
                     const std::string& host_name,double metric_value);
  void publishMetricByHTTPRequest(const std::string& metricName, const std::string& application_name,
                                  const std::string& host_name,double metric_value);
  void ccmPublishMetric(const std::string& metricName, const std::string& application_name,
                        const std::string& host_name,double metric_value);    
  const int getPort();
  const std::string& getDatabaseName();
  const std::string& getInfluxDbUrl();
    
private:
  int port;
  std::string database_name;
  std::string influxdb_uri;
};

#endif /* METRIC_PUBLISH_H_ */





