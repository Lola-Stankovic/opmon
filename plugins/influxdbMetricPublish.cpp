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


using namespace std::chrono_literals;
using namespace std;
using namespace std::chrono;

class influxdbMetricPublish : public MetricPublish
{
public:
  explicit influxdbMetricPublish(std::map<std::string, std::string> parameters)
  {
    setPublisher(parameters["portNumber"], parameters["databaseName"], parameters["influxdbUri"]);
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
