/**
 * @file OpMonManager.hpp
 *
 * This is part of the DUNE DAQ Application Framework, copyright 2020.
 * Licensing/copyright details are in the COPYING file that you should have
 * received with this code.
 */

#ifndef OPMONLIB_INCLUDE_OPMONLIB_OPMONMANAGER_HPP_
#define OPMONLIB_INCLUDE_OPMONLIB_OPMONMANAGER_HPP_

#include "opmonlib/MonitorableObject.hpp"

namespace dunedaq {

  ERS_DECLARE_ISSUE( opmonlib,
		     ThreadNameTooLong,
		     "The name " << name << " is too long for a thread name",
		     ((std::string)name)
		     )

  ERS_DECLARE_ISSUE( opmonlib,
		     ErrorWhileCollecting,
		     "OpMon data collection failed",
		     ERS_EMPTY )
}

namespace dunedaq::opmonlib {

class OpMonManager : public MonitorableObject 
{
public:

  explicit OpMonManager(std::string session,
			std::string name,
			std::string opmon_facility_uri);
  ~OpMonManager();

  using MonitorableObject::register_child ;
  
  // data collecting loop
  void start(std::chrono::seconds, opmon_level); 
  void stop();

protected:
  void run(std::chrono::seconds, opmon_level ); // function used by thread

private:

  std::jthread m_thread;

};

} // namespace dunedaq::opmonlib

#endif // OPMONLIB_INCLUDE_OPMONLIB_OPMONMANAGER_HPP_
