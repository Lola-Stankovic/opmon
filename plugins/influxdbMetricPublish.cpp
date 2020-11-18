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

using namespace dunedaq::opmonlib;
using namespace std::chrono_literals;

class influxdbMetricPublish : public MetricPublish
{
public:
  explicit influxdbMetricPublish(std::map<std::string, std::string> par)
  {
    std::string uri;
    uri = std::string("http://" + par["influxdbUri"] + ":" + par["databaseName"] +
      "/write?db=" + par["portNumber"]);
    setParameters(std::stoi(par["portNumber"]), par["databaseName"], par["influxdbUri"]);
  }

  void
  setParameters(int portNumber, const std::string& databaseName,
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
    std::cout << "Publishing metrics to a influx database!" << std::endl;
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
  std::shared_ptr<dunedaq::opmonlib::MetricPublish> make(std::map<std::string, std::string> uri) {
      return std::shared_ptr<dunedaq::opmonlib::MetricPublish>(new influxdbMetricPublish(uri));
  }
}
