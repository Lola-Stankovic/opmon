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
  explicit fileMetricPublish(std::map<std::string, std::string> par)
  {
    std::string uri;
    uri = std::string(par["fileName"]);
    auto col = uri.find_last_of(':');
    auto sep = uri.find("://");
    std::string fname;
    if (col == std::string::npos || sep == std::string::npos) { // assume filename
      fname = uri;
    } else {
      fname = uri.substr(sep+3);
    }
    std::cout << fname << "\n";
    setFile(fname);
    try {
      std::ifstream ifs;
      ifs.open(fname, std::fstream::in);
      if (!ifs.is_open()) {
        throw dunedaq::opmonlib::FilePublishError(ERS_HERE, "Can't open file!");
      }
      raw_commands_ = json::parse(ifs);
    } catch (const std::exception& ex) {
      throw dunedaq::opmonlib::FilePublishError(ERS_HERE, ex.what());
    }
  }
  
  void
  setFile(const std::string& fileName)
  {
    file_name_ = fileName;
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

    std::cout << logging.dump(4) << std::endl;
    std::cout << std::setw(4) << logging << endl; 

    //std::ofstream output_file(std::string(file_name_));
    //output_file << logging;

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

};

extern "C" {
  std::shared_ptr<dunedaq::opmonlib::MetricPublish> make(std::map<std::string, std::string> uri) {
      return std::shared_ptr<dunedaq::opmonlib::MetricPublish>(new fileMetricPublish(uri));
  }
}
