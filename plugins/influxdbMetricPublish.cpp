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
#include "influxdb.hpp"

#include <ers/ers.h>
#include <cetlib/BasicPluginFactory.h>

using namespace dunedaq::opmlib;
using namespace std::chrono_literals;

class influxdbMetricPublish : public MetricPublish
{
public:
  explicit influxdbMetricPublish(std::string uri)
  {
    auto col = uri.find_last_of(':');
    auto sep = uri.find("://");
    std::string fname;
    if (col == std::string::npos || sep == std::string::npos) { // assume filename
      fname = uri;
    } else {
      fname = uri.substr(sep+3);
    }
    std::cout << fname;
    setPublisher(8086, "prometheus_lola", "localhost");
  }

  uint64_t
  timeSinceEpochMillisec()
  {
    using namespace std::chrono;
    return duration_cast<nanoseconds>(system_clock::now().time_since_epoch()).count();
  }
  void
  setPublisher(int portNumber, const std::string& databaseName,
               const std::string& influxdbUri)
  {
    port_ = portNumber;
    database_name_ = databaseName;
    influxdb_uri_ = influxdbUri;
  }	 

  void
  publishMetric(const std::string& metricName, const std::string& application_name,
                const std::string& host_name,double metric_value)
  {
    influxdb_cpp::server_info si("127.0.0.1", getPort(), getDatabaseName());
    influxdb_cpp::builder()
    .meas(metricName)
    .tag("host",  host_name)
    .tag("application", application_name)
    .field("x", metric_value)
    .timestamp(timeSinceEpochMillisec())
    .post_http(si);
  }
  const int
  getPort()
  {
    return port_;
  }

  const std::string&
  getDatabaseName()
  {
    return database_name_;
  }

  std::string&
  getInfluxDbUrl()
  {
    return influxdb_uri_;
  }
 

protected:
  typedef MetricPublish inherited;
  int port_;
  std::string database_name_;
  std::string influxdb_uri_;


};

extern "C" {
  std::shared_ptr<dunedaq::opmlib::MetricPublish> make(std::string uri) {
      return std::shared_ptr<dunedaq::opmlib::MetricPublish>(new influxdbMetricPublish(uri));
  }
}
