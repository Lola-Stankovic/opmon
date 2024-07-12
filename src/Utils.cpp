/**
 * @file Utils.cpp
 *
 * This is part of the DUNE DAQ Application Framework, copyright 2020.
 * Licensing/copyright details are in the COPYING file that you should have
 * received with this code.
 */

#include "opmonlib/Utils.hpp"

dunedaq::opmon::OpMonEntry dunedaq::opmonlib::to_entry(const google::protobuf::Message & m,
						       const CustomOrigin & co) {

  dunedaq::opmon::OpMonEntry entry;
  entry.set_measurement( m.GetTypeName() );

  *entry.mutable_data() = dunedaq::opmonlib::to_map(m);
  
  for ( const auto & [tag, value] : co ) {
    (*entry.mutable_custom_origin())[tag] = value;
  }
  
  return entry;
}


dunedaq::opmonlib::map_type dunedaq::opmonlib::to_map( const google::protobuf::Message & m,
						       std::string top_block) {

  dunedaq::opmonlib::map_type ret;

  const auto * descriptor_p = m.GetDescriptor();
  const auto & des = *descriptor_p;
  
  const auto * reflection_p = m.GetReflection();
  const auto & ref = *reflection_p;
  
  using namespace google::protobuf;

  auto count = des.field_count();
  for ( int i = 0; i < count; ++i ) {
    const auto * field_p = des.field(i);
    if ( field_p -> is_repeated() ) continue;
    
    auto name = top_block.empty() ? field_p -> name() : top_block + '.' + field_p -> name(); 

    auto type = field_p -> cpp_type();
    dunedaq::opmon::OpMonValue value;
    bool success = true;
    switch (type) {
    case FieldDescriptor::CppType::CPPTYPE_INT32:
      value.set_int4_value( ref.GetInt32(m, field_p) );
      break;
    case FieldDescriptor::CppType::CPPTYPE_INT64:
      value.set_int8_value( ref.GetInt64(m, field_p) );
      break;
    case FieldDescriptor::CppType::CPPTYPE_UINT32:
      value.set_uint4_value( ref.GetUInt32(m, field_p) );
      break;
    case FieldDescriptor::CppType::CPPTYPE_UINT64:
      value.set_uint8_value( ref.GetUInt64(m, field_p) );
      break;
    case FieldDescriptor::CppType::CPPTYPE_DOUBLE:
      value.set_double_value( ref.GetDouble(m, field_p) );
      break;
    case FieldDescriptor::CppType::CPPTYPE_FLOAT:
      value.set_float_value( ref.GetFloat(m, field_p) );
      break;
    case FieldDescriptor::CppType::CPPTYPE_BOOL:
      value.set_boolean_value( ref.GetBool(m, field_p) );
      break;
    case FieldDescriptor::CppType::CPPTYPE_STRING:
      value.set_string_value( ref.GetString(m, field_p) );
      break;
    case FieldDescriptor::CppType::CPPTYPE_MESSAGE:
      {
	auto data = dunedaq::opmonlib::to_map(ref.GetMessage(m, field_p), name);
	ret.insert(data.begin(), data.end());
	success = false;
	break;
      }
    default:
      success = false;
      break;
    } // switch on the cpp type                                                                         

      
    if ( success )
      ret[name] = value;
  }

  return ret;
  
}

std::string dunedaq::opmonlib::to_string( const dunedaq::opmon::OpMonId & id ) {

  std::string ret(id.session());

  if ( ! id.application().empty() ) {
    ret += '.';
    ret += id.application();
  }
  
  for( int i = 0; i < id.substructure().size(); ++i ) {
    ret += '.';
    ret += id.substructure()[i];
  }

  return ret;
}


const dunedaq::opmon::OpMonId & dunedaq::opmonlib::operator += (dunedaq::opmon::OpMonId & id,
								const  std::string & element) {
  
  if ( element.empty() ) return id;
  
  if ( id.session().empty() ) {
    id.set_session(element);
    return id;
  }

  if ( id.application().empty() ) {
    id.set_application(element);
    return id;
  }

  id.add_substructure(element);
  return id;
}


dunedaq::opmon::OpMonId dunedaq::opmonlib::operator + (const dunedaq::opmon::OpMonId & id,
						       const  std::string & element ) {

  auto ret = id;
  ret += element;
  return ret;
}

