/**
 * @file Utils.hpp
 *
 * This is part of the DUNE DAQ Application Framework, copyright 2020.
 * Licensing/copyright details are in the COPYING file that you should have
 * received with this code.
 */

#ifndef OPMONLIB_INCLUDE_OPMONLIB_UTILS_HPP_
#define OPMONLIB_INCLUDE_OPMONLIB_UTILS_HPP_


#include <string>
#include <list>
#include <map>

#include <google/protobuf/message.h>
#include "opmonlib/opmon_entry.pb.h"

#include <google/protobuf/util/time_util.h>

namespace dunedaq::opmonlib {

  using CustomOrigin = std::map<std::string, std::string> ;
  using cr_map_type = std::invoke_result<decltype(&dunedaq::opmon::OpMonEntry::data),
					 dunedaq::opmon::OpMonEntry>::type;
  using map_type = std::remove_const<std::remove_reference<cr_map_type>::type>::type;
  
  dunedaq::opmon::OpMonEntry to_entry(const google::protobuf::Message & m,
				      const CustomOrigin & co);

  template<class M>
  M from_entry( const dunedaq::opmon::OpMonEntry & );
  
  map_type to_map(const google::protobuf::Message & m,
		  std::string top_block = "");

  std::string to_string( const dunedaq::opmon::OpMonId & );

  const dunedaq::opmon::OpMonId & operator += (dunedaq::opmon::OpMonId &,
					       const  std::string & element);
  
  dunedaq::opmon::OpMonId operator + (const dunedaq::opmon::OpMonId &,
				      const  std::string & element ) ;

}  // namespace dunedaq::opmonlib

#include "details/Utils.hxx"

#endif // OPMONLIB_INCLUDE_OPMONLIB_UTILS_HPP_
