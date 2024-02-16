/**
 * @file OpMonFacility.hpp
 *
 * This is part of the DUNE DAQ Application Framework, copyright 2020.
 * Licensing/copyright details are in the COPYING file that you should have
 * received with this code.
 */

#ifndef OPMONLIB_INCLUDE_OPMONLIB_OPMONFACILITY_HPP_
#define OPMONLIB_INCLUDE_OPMONLIB_OPMONFACILITY_HPP_

#include "Issues.hpp"

#include "logging/Logging.hpp"

#include <cetlib/BasicPluginFactory.h>
#include <cetlib/compiler_macros.h>
#include "opmonlib/opmon_entry.pb.h"

#include <iostream>
#include <memory>
#include <string>

#ifndef EXTERN_C_FUNC_DECLARE_START
// NOLINTNEXTLINE(build/define_used)
#define EXTERN_C_FUNC_DECLARE_START                                                                                    \
  extern "C"                                                                                                           \
  {
#endif

/**
 * @brief Declare the function that will be called by the plugin loader
 * @param klass Class to be defined as a Operational Monitoring Facility
 */

// NOLINTNEXTLINE(build/define_used)
#define DEFINE_DUNE_OPMON_FACILITY(klass)                                                                               \
  EXTERN_C_FUNC_DECLARE_START                                                                                          \
  std::shared_ptr<dunedaq::opmonlib::OpMonFacility> make(std::string facility)                                           \
  {                                                                                                                    \
    return std::shared_ptr<dunedaq::opmonlib::OpMonFacility>(new klass(facility));                                       \
  }                                                                                                                    \
  }

namespace dunedaq::opmonlib {

class OpMonFacility
{
public:
  explicit OpMonFacility(std::string /*service*/) {}
  virtual ~OpMonFacility() = default;
  OpMonFacility(const OpMonFacility&) = delete;            ///< OpMonFacility is not copy-constructible
  OpMonFacility& operator=(const OpMonFacility&) = delete; ///< OpMonFacility is not copy-assignable
  OpMonFacility(OpMonFacility&&) = delete;                 ///< OpMonFacility is not move-constructible
  OpMonFacility& operator=(OpMonFacility&&) = delete;      ///< OpMonFacility is not move-assignable

  // Publish information
  virtual void publish(opmon::OpMonEntry &&) = 0;

private:
};

std::shared_ptr<OpMonFacility>
makeOpMonFacility(std::string const& facility)
{
  TLOG() << "FACILITY = " << facility;
  auto sep = facility.find("://");
  std::string scheme;
  if (sep == std::string::npos) { // simple path
    scheme = "stdout";
  } else { // with scheme
    scheme = facility.substr(0, sep);
  }
  std::string plugin_name = scheme + "OpMonFacility";
  static cet::BasicPluginFactory bpf("duneOpMonFacility", "make");
  std::shared_ptr<OpMonFacility> os_ptr;
  try {
    os_ptr = bpf.makePlugin<std::shared_ptr<OpMonFacility>>(plugin_name, facility);
  } catch (const cet::exception& cexpt) {
    throw OpMonFacilityCreationFailed(ERS_HERE, plugin_name, cexpt);
  } catch (const ers::Issue& iexpt) {
    throw OpMonFacilityCreationFailed(ERS_HERE, plugin_name, iexpt);
  } catch (...) { // NOLINT JCF Jan-27-2021 violates letter of the law but not the spirit
    throw OpMonFacilityCreationFailed(ERS_HERE, plugin_name);
  }
  return os_ptr;
}

} // namespace dunedaq::opmonlib

#endif // OPMONLIB_INCLUDE_OPMONLIB_OPMONFACILITY_HPP_
