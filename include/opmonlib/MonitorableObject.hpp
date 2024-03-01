/**
 * @file MonitorableObject.hpp
 *
 * This is part of the DUNE DAQ Application Framework, copyright 2020.
 * Licensing/copyright details are in the COPYING file that you should have
 * received with this code.
 */

#ifndef OPMONLIB_INCLUDE_OPMONLIB_MONITORABLEOBJECT_HPP_
#define OPMONLIB_INCLUDE_OPMONLIB_MONITORABLEOBJECT_HPP_

#include "opmonlib/OpMonFacility.hpp"
#include <google/protobuf/message.h>

#include <map>
#include <memory>
#include <string>
#include <mutex>

namespace dunedaq {

  ERS_DECLARE_ISSUE( opmonlib,
		     NonUniqueChildName,
		     name << " already present in the child list",
		     ((std::string)name)
		     )
	  
  ERS_DECLARE_ISSUE( opmonlib,
		     EntryWithNoData,
		     "OpMonEntry of type " << type << " has no data",
		     ((std::string)type)
		   )

}


namespace dunedaq::opmonlib {

class MonitorableObject
{
public:

  using child_ptr = std::weak_ptr<MonitorableObject>;
  using new_child_ptr = std::shared_ptr<MonitorableObject>;
  using opmon_level = uint32_t; // NOLINT(build/unsigned)     
  using opmon_id = std::string;
  
  opmon_id get_opmon_id() const noexcept {
    return m_parent_opmon_id.empty() ? m_opmon_name : m_parent_opmon_id + '.' + m_opmon_name; }

protected:

  /**
    * Append a register object to the chain
    * The children will be modified using information from the this parent
    */
  void register_child( std::string name, new_child_ptr ) ;

  /**
   * Convert the message into an OpMonEntry and then uses the pointer to the Facility to publish the entry
   * This also timestamps the message with the time of the invocation
   */
  void publish( google::protobuf::Message && ) const noexcept ;

  /**
   * Instructs the object to pusblish regular interval metrics.
   * It also instruct the children to execute their collect methods.
   */     
  void collect(opmon_level) ;

  /**
   * Hook for customisable pubblication
   */
  virtual void generate_opmon_data() {;}
    
private:

  /**
   * utilities for linking with parent and top levels
   */
  void inherit_parent_properties( const MonitorableObject & parent );   // funcion called on the children as well

  std::map<std::string, child_ptr> m_children ;
  std::mutex m_children_mutex;

  std::shared_ptr<opmonlib::OpMonFacility> m_facility = makeOpMonFacility("null://");
  opmon_id m_parent_opmon_id;
  opmon_id m_opmon_name;
};

} // namespace dunedaq::opmonlib

#endif // OPMONLIB_INCLUDE_OPMONLIB_MONITORABLEOBJECT_HPP_
