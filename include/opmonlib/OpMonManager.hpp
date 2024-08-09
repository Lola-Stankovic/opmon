/**
 * @file OpMonManager.hpp
 *
 * This is part of the DUNE DAQ Application Framework, copyright 2020.
 * Licensing/copyright details are in the COPYING file that you should have
 * received with this code.
 */

#ifndef OPMONLIB_INCLUDE_OPMONLIB_OPMONMANAGER_HPP_
#define OPMONLIB_INCLUDE_OPMONLIB_OPMONMANAGER_HPP_

#include <cstddef>
#include "opmonlib/MonitorableObject.hpp"
#include "utilities/WorkerThread.hpp" 

#include "opmonlib/BackendOpMonFacility.hpp"

namespace dunedaq {

  ERS_DECLARE_ISSUE( opmonlib,
                     MonitoringThreadNotSet,
                     "Monitoring thread not set",
                     ERS_EMPTY )

  ERS_DECLARE_ISSUE( opmonlib,
		     FailedMonitoringThread,
		     "Monitoring thread failed to start",
		     ERS_EMPTY )
}



namespace dunedaq::opmonlib {

class OpMonManager : protected MonitorableObject 
{
public:

  /*
   * This constructor is for test purpose and initialise the facility to the backend facility
   */
  explicit OpMonManager(nullptr_t) :
    MonitorableObject( "NULL", "tree") { m_facility = std::make_shared<BackendOpMonFacility>();}

  /*
   * Construtor expected to be used for standard operations, 
   * yet, general developers should not concern themselves with this as the framework handles this for them
   */
  explicit OpMonManager(std::string session,
			std::string name,
			std::string opmon_facility_uri = "stdout");
  
  virtual ~OpMonManager();

  using MonitorableObject::get_opmon_id;
  using MonitorableObject::get_opmon_level;
  using MonitorableObject::register_node;
  using MonitorableObject::set_opmon_level;
  
  // data collecting loop
  void start_monitoring(std::chrono::seconds); 
  void stop_monitoring();

  //obtain the opmon facility
  auto get_opmon_facility() { return m_facility; }
  auto get_backend_facility() { return std::dynamic_pointer_cast<BackendOpMonFacility>(m_facility);  }
  
protected:
  using MonitorableObject::collect;
  
  void run( std::atomic<bool> & running,
	    std::chrono::seconds ); // function used by thread

private:

  std::unique_ptr<utilities::WorkerThread> m_thread_p;

};

 
} // namespace dunedaq::opmonlib

#endif // OPMONLIB_INCLUDE_OPMONLIB_OPMONMANAGER_HPP_
