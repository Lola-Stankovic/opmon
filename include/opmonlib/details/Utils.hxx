template<class M>
M dunedaq::opmonlib::from_entry(const dunedaq::opmon::OpMonEntry & e ) {

  M ret;

  if ( ret.GetTypeName() != e.measurement() ) {
    throw opmonlib::NameMismatch(ERS_HERE, ret.GetTypeName(), e.measurement());
  }

  from_entry(ret, e);
  
  return ret;
}


template<class T>
void dunedaq::opmonlib::set_value(google::protobuf::Message & m, const std::string & name, T value) {

  const auto * descriptor_p = m.GetDescriptor();
  const auto & des = *descriptor_p;

  auto field_p = des.FindFieldByName(name);
  if ( ! field_p ) {
    throw MissingField(ERS_HERE, name, m.GetTypeName());
  }

  auto type = field_p -> cpp_type();
  
  const auto * reflection_p = m.GetReflection();
  const auto & ref = *reflection_p;

  set_value( ref, m, field_p, value );

  // switch (type) {
  // case FieldDescriptor::CppType::CPPTYPE_INT32:
  //   ref.SetInt32(&m, field_p, value);
  //   break;
  // case FieldDescriptor::CppType::CPPTYPE_INT64:
  //   ref.SetInt64(&m, field_p, value);
  //   break;
  // case FieldDescriptor::CppType::CPPTYPE_UINT32:
  //   ref.SetUInt32(&m, field_p, value);
  //   break;
  // case FieldDescriptor::CppType::CPPTYPE_UINT64:
  //   ref.SetUInt64(&m, field_p, value);
  //   break;
  // case FieldDescriptor::CppType::CPPTYPE_DOUBLE:
  //   ref.SetDouble(&m, field_p, value);
  //   break;
  // case FieldDescriptor::CppType::CPPTYPE_FLOAT:
  //   ref.SetFloat(&m, field_p, value);
  //   break;
  // case FieldDescriptor::CppType::CPPTYPE_BOOL:
  //   ref.SetBool(&m, field_p, value);
  //   break;
  // case FieldDescriptor::CppType::CPPTYPE_STRING:
  //   ref.SetString(&m, field_p, value);
  //   break;
  // default:
  //   break;
  // } // switch on the cpp type
  
}
