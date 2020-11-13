#include <iostream>
#include <string>
#include <map>
#include <unistd.h>
#include <future>
#include <fstream>
#include <memory>
#include <set>
#include <chrono>
#include <cstdint>
#include <stdio.h>
#include <iomanip>

#include "opmlib/MetricPublish.hpp"
#include "influxdb.hpp"

#include <ers/ers.h>
#include <nlohmann/json.hpp>
#include <cetlib/BasicPluginFactory.h>

using namespace dunedaq::opmlib;
using namespace std::chrono_literals;
using json = nlohmann::json;

class fileMetricPublish : public MetricPublish
{
public:
  explicit fileMetricPublish(std::string uri)
  {
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
    /*try {
      std::ifstream ifs;
      ifs.open(fname, std::fstream::in);
      if (!ifs.is_open()) {
        throw std::invalid_argument("Can't open file.");
      }
      raw_commands_ = json::parse(ifs);
    } catch (const std::exception& ex) {
      //throw dunedaq::cmdlib::CommandParserError(ERS_HERE, ex.what());
    }*/
  }

  uint64_t
  timeSinceEpochMillisec()
  {
    using namespace std::chrono;
    return duration_cast<nanoseconds>(system_clock::now().time_since_epoch()).count();
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
    /*char hostname[HOST_NAME_MAX];
    char username[LOGIN_NAME_MAX];
    gethostname(hostname, HOST_NAME_MAX);
    getlogin_r(username, LOGIN_NAME_MAX);*/	
    json logging;
    logging["user"] = "username";
    logging["machine"] = "hostname";
    logging["timestamp"] = timeSinceEpochMillisec();
    logging["metric_name"] = metricName;
    logging["application_name"] = application_name;
    logging["host_name"] = host_name;
    logging["metric_value"] = metric_value;

    std::ofstream output_file(std::string(file_name_));
    std::cout << logging.dump(4) << std::endl;
    std::cout << std::setw(4) << logging << std::endl;

    //output_file << std::setw(4) << logging << std::endl;

  }

  const std::string&
  getFileName()
  {
    return file_name_;
  }

protected:
  typedef MetricPublish inherited;
  std::string file_name_;

};

extern "C" {
  std::shared_ptr<dunedaq::opmlib::MetricPublish> make(std::string uri) {
      return std::shared_ptr<dunedaq::opmlib::MetricPublish>(new fileMetricPublish(uri));
  }
}
