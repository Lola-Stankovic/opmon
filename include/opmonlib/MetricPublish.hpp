#ifndef OPMONLIB_INCLUDE_OPMONLIB_METRICPUBLISH_HPP_
#define OPMONLIB_INCLUDE_OPMONLIB_METRICPUBLISH_HPP_

#include <cetlib/BasicPluginFactory.h>
#include <cetlib/compiler_macros.h>

#include <iostream>
#include <string>
#include <functional>
#include <future>
#include <memory>
#include <thread>
#include <set>

#ifndef EXTERN_C_FUNC_DECLARE_START
#define EXTERN_C_FUNC_DECLARE_START                                                                                    \
  extern "C"                                                                                                           \
  {
#endif

/**
 *  * @brief Declare the function that will be called by the plugin loader
 *   * @param klass Class to be defined as a DUNE Metric Publish
 *    */
#define DEFINE_DUNE_METRIC_PUBLISH(klass)                                                                            \
  EXTERN_C_FUNC_DECLARE_START                                                                                          \
  std::unique_ptr<dunedaq::opmlib::MetricPublish> make()                                                             \
  {                                                                                                                    \
    return std::unique_ptr<dunedaq::opmlib::MetricPublish>(new klass());                                             \
  }                                                                                                                    \
  }

namespace dunedaq::opmonlib {

class MetricPublish
{
public:
  explicit MetricPublish( ) { }
  ~MetricPublish() { }
  virtual void publishMetric(const std::string& metricName, const std::string& application_name,
                     const std::string& host_name,double metric_value) = 0;
  uint64_t timeSinceEpochMillisec();
        
};

/*
std::shared_ptr<MetricPublish>
makeMetricPublish(std::string const& uri)
{
  std::string source = "influxdb";
  std::string plugin_name = source + "MetricPublish";
  static cet::BasicPluginFactory bpf("duneMetricPublish", "make"); 
  std::shared_ptr<MetricPublish> cf_ptr;
  try {
    cf_ptr = bpf.makePlugin<std::shared_ptr<MetricPublish>>(plugin_name, uri);
  } catch (const cet::exception &cexpt) {
    //throw CommandFacilityCreationFailed(ERS_HERE, uri, cexpt);
  }
  return cf_ptr;

}*/

}

#endif
