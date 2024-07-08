/**
 * @file MonitorableObject.hpp
 *
 * This is part of the DUNE DAQ Application Framework, copyright 2020.
 * Licensing/copyright details are in the COPYING file that you should have
 * received with this code.
 */

#ifndef OPMONLIB_INCLUDE_OPMONLIB_MONITORABLEOBJECT_HPP_
#define OPMONLIB_INCLUDE_OPMONLIB_MONITORABLEOBJECT_HPP_

#include "opmonlib/Utils.hpp"
#include "opmonlib/OpMonFacility.hpp"
#include <google/protobuf/message.h>
#include <opmonlib/info/MonitoringTreeInfo.pb.h>

#include <limits>
#include <type_traits>
#include <map>
#include <memory>
#include <string>
#include <mutex>

namespace dunedaq {

  ERS_DECLARE_ISSUE( opmonlib,
		     NonUniqueChildName,
		     name << " already present in the child list of " << opmon_id,
		     ((std::string)name)((std::string)opmon_id) 
		     )
	  
  ERS_DECLARE_ISSUE( opmonlib,
		     EntryWithNoData,
		     "OpMonEntry of type " << type << " has no data",
		     ((std::string)type)
		   )

  ERS_DECLARE_ISSUE( opmonlib,
                     ErrorWhileCollecting,
                     "OpMon data collection failed in " << opmon_id,
                     ((std::string)opmon_id) )

}


namespace dunedaq::opmonlib {

  using OpMonLevel = unsigned int;
  
  enum class SystemOpMonLevel : OpMonLevel {
    kDisabled = std::numeric_limits<OpMonLevel>::min(),
    kAll      = std::numeric_limits<OpMonLevel>::max()
  };

  enum class EntryOpMonLevel : OpMonLevel {
    kTopPriority     = std::numeric_limits<OpMonLevel>::min(),
    kAsync           = std::numeric_limits<OpMonLevel>::max()/4,
    kDefault         = std::numeric_limits<OpMonLevel>::max()/2,
    kEventDriven     = (std::numeric_limits<OpMonLevel>::max()/4)*3,
    kLowestPrioriry  = std::numeric_limits<OpMonLevel>::max()-1
  };

  template <class T>
  auto to_level( T v ) {
    return static_cast<typename std::underlying_type<T>::type>(v);
  }
  
  
class MonitorableObject
{
public:

  using child_ptr = std::weak_ptr<MonitorableObject>;
  using new_child_ptr = std::shared_ptr<MonitorableObject>;
  using element_id = std::string;

  friend class OpMonManager;
  
  /**
   * copy and move constructors are deleted as they violate the linking chain
   */
  MonitorableObject( const MonitorableObject & ) = delete ;
  MonitorableObject & operator = ( const MonitorableObject &) = delete;
  MonitorableObject( MonitorableObject && ) = delete;
  MonitorableObject & operator = (MonitorableObject &&) = delete;    
  
  virtual ~MonitorableObject() {;}
  
  auto get_opmon_id() const noexcept { return m_parent_id + m_opmon_name; }

  auto get_opmon_level() const noexcept { return m_opmon_level.load(); }
  
  static bool publishable_metric( OpMonLevel entry, OpMonLevel system ) noexcept {
    return (entry < system);
  }
  
protected:

  /**
   * default constructors are ok as they set the links correctly
   * i.e. The service points to the null and the names are not set
   */
  MonitorableObject() = default;

  /**
    * Append a register object to the chain
    * The children will be modified using information from the this parent
    */
  void register_child( std::string name, new_child_ptr ) ;

  /**
   * Convert the message into an OpMonEntry and then uses the pointer to the Facility to publish the entry.
   * This also timestamps the message with the time of the invocation.
   * It is possible to associate an element name to the published message.
   *    the element name is checked against the children to protect uniqueness. 
   * It is also possible to associate a custom origin in the form of a map<string,string>. 
   *    This is designed to add information which is independent from the software structure
   *    e.g.  channels or other hardware information.
   * Messages will have an associated OpmonLevel that is used to suppress the pubblication of metrics. 
   *    The level of the message is set by the OpMonManager.
   *    
   */
  void publish( google::protobuf::Message &&,
		CustomOrigin && co = {},
		OpMonLevel l = to_level(EntryOpMonLevel::kDefault),
		const element_id & element = "" ) const noexcept ;

  /**
   * Hook for customisable pubblication. 
   * The function can throw, exception will be caught by the monitoring thread
   */
  virtual void generate_opmon_data() {return;}

  
private:

  /**
   * Instructs the object to pusblish regular interval metrics.
   * It also instruct the children to execute their collect methods.
   * 
   * \return It returns a protobuf schema object to monitor the tree
   */     
  opmon::MonitoringTreeInfo collect() noexcept ;
  
  /**
   * Hook to propagate the OpMonLevel at lower levels of the monitoring tree
   */
  void set_opmon_level( OpMonLevel ) noexcept; 

  /**
   * utilities for linking with parent and top levels
   */
  void inherit_parent_properties( const MonitorableObject & parent );   // funcion called on the children as well

   /**
   * Contructor to set initial strings
   */ 
  MonitorableObject( element_id name, element_id parent_id = "" )
    : m_parent_id()
    , m_opmon_name(name) {
    m_parent_id.set_session(parent_id);
  }
  
  std::map<std::string, child_ptr> m_children ;
  std::mutex m_children_mutex;

  std::shared_ptr<opmonlib::OpMonFacility> m_facility = makeOpMonFacility("null://");
  dunedaq::opmon::OpMonId m_parent_id;
  std::atomic<OpMonLevel> m_opmon_level = to_level(SystemOpMonLevel::kAll);
  element_id m_opmon_name;

  // info for monitoring the monitoring structure
  using const_metric_counter_t = std::invoke_result<decltype(&dunedaq::opmonlib::opmon::MonitoringTreeInfo::n_published_measurements),
						    dunedaq::opmonlib::opmon::MonitoringTreeInfo>::type;
  using metric_counter_t = std::remove_const<const_metric_counter_t>::type;
  mutable std::atomic<metric_counter_t> m_published_counter{0};
  mutable std::atomic<metric_counter_t> m_ignored_counter{0};
  mutable std::atomic<metric_counter_t> m_error_counter{0};
};

} // namespace dunedaq::opmonlib

#endif // OPMONLIB_INCLUDE_OPMONLIB_MONITORABLEOBJECT_HPP_
