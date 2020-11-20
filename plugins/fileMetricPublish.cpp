#include <map>
#include <fstream>
#include <string>
#include <memory>
#include <set>
#include <chrono>
#include <cstdint>
#include <stdio.h>
#include <iomanip>
#include <sstream>

#include "opmonlib/MetricPublish.hpp"
#include "opmonlib/Issues.hpp"
#include "influxdb.hpp"

#include <ers/ers.h>
#include <nlohmann/json.hpp>
#include <cetlib/BasicPluginFactory.h>

using namespace dunedaq::opmonlib;
using namespace std::chrono_literals;
using namespace std;
using json = nlohmann::json;

class fileMetricPublish : public MetricPublish
{
public:
  explicit fileMetricPublish(const std::map<std::string, std::string>& par)
  {
    file_name_ = par.at("fileName");
    try {
      ofs_.open(file_name_, std::ios::out);
      if (!ofs_.is_open()) {
        throw dunedaq::opmonlib::FilePublishError(ERS_HERE, "Can't open file!");
      }
    } catch (const std::exception& ex) {
      throw dunedaq::opmonlib::FilePublishError(ERS_HERE, ex.what());
    }
  }
  
  void
  publishMetric(const std::string& metricName, const std::string& application_name,
                const std::string& host_name,double metric_value)
  {
    std::cout << "Publishing metrics to a file!" << std::endl;	
    json logging;
    logging["user"] = "username";
    logging["machine"] = "hostname";
    logging["timestamp"] = timeSinceEpochMillisec();
    logging["metric_name"] = metricName;
    logging["application_name"] = application_name;
    logging["host_name"] = host_name;
    logging["metric_value"] = metric_value;

    /*std::cout << logging.dump(4) << std::endl;
    std::cout << std::setw(4) << logging << endl;*/ 

    ofs_ << logging.dump(4) << std::endl; //4 spaces to indent

  }

  const std::string&
  getFileName()
  {
    return file_name_;
  }

protected:
  typedef MetricPublish inherited;
  
  json raw_commands_;
  std::string file_name_;
  std::ofstream ofs_;

};

extern "C" {
  std::shared_ptr<dunedaq::opmonlib::MetricPublish> make(std::map<std::string, std::string> uri) {
      return std::shared_ptr<dunedaq::opmonlib::MetricPublish>(new fileMetricPublish(uri));
  }
}
