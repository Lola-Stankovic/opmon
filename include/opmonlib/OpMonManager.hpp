/**
 * @file OpMonManager.hpp
 *
 * This is part of the DUNE DAQ Application Framework, copyright 2020.
 * Licensing/copyright details are in the COPYING file that you should have
 * received with this code.
 */

#ifndef OPMONLIB_INCLUDE_OPMONLIB_OPMONMANAGER_HPP_
#define OPMONLIB_INCLUDE_OPMONLIB_OPMONMANAGER_HPP_

#include "opmonlib/OpMonFacility.hpp"

#include <memory>
#include <string>
#include <thread>
#include <chrono>
#include <list>

namespace dunedaq::opmonlib {

  class MonitorableObject;

  using	opmon_level = uint32_t; // NOLINT(build/unsigned)                                                             


  
class OpMonManager
{
public:

  using child_ptr = std::weak_ptr<MonitorableObject>;
  
  explicit OpMonManager(std::string name,
			std::string service); // Constructor
  
  // data collecting loop
  void start(std::chrono::seconds, opmon_level); // NOLINT(build/unsigned)
  void stop();

protected:
  void collect(opmon_level) ;  // instruct the registered children to publish regular interval metrics  
  
private:

  std::shared_ptr<opmonlib::OpmonService> m_service;
  std::list<child_ptr> m_children ;

  std::jthread m_thread;
  void run(std::chrono::seconds, opmon_level ); // function used by thread

};

} // namespace dunedaq::opmonlib

#endif // OPMONLIB_INCLUDE_OPMONLIB_OPMONMANAGER_HPP_
