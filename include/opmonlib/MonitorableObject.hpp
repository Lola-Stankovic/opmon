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

#include <list>
#include <memory>
#include <string>
#include <mutex>

namespace dunedaq::opmonlib {

class MonitorableObject
{
public:

  using child_ptr = std::weak_ptr<MonitorableObject>;
  using opmon_level = uint32_t; // NOLINT(build/unsigned)     
  using opmon_id = std::string;
  
  opmon_id get_opmon_id() noexcept const { return m_id; }

protected:

  /**
    * Append a register object to the chain
    * The children will be modified using information from the this parent
    */
  void register_child( std::string name, child_ptr ) ;

  /**
   * Convert the message into an OpMonEntry and then uses the pointer to the Facility to publish the entry
   */
  bool publish( google::protobuf::Message && ) const;

  /**
   * Instructs the object to pusblish regular interval metrics.
   * It also instruct the children to execute their collect methods.
   */     
  void collect(opmon_level) ;

  /**
   * Hook for customisable pubblication
   */
  virtual void generate_opmon_data() {;}

  /**
   * hook to add children in the chain
   */
  void register_child( std::string name, child_ptr ) ;
    
private:

  /**
   * utilities for linking with parent and top levels
   */
  void inherit_parent_properties( const MonitorableObject & parent );
  opmon_id set_opmon_id( opmon_id parent, opmon_id name);

  std::list<child_ptr> m_children ;
  std::mutex m_children_mutex;

  std::shared_ptr<opmonlib::OpmonService> m_service;
  opmon_id m_id;

};

} // namespace dunedaq::opmonlib

#endif // OPMONLIB_INCLUDE_OPMONLIB_MONITORABLEOBJECT_HPP_
