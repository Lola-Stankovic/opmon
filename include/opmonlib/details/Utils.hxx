template<class M>
M dunedaq::opmonlib::from_entry(const dunedaq::opmon::OpMonEntry & e ) {

  M ret;

  if ( ret.GetTypeName() != e.measurement() ) {
    throw opmonlib::NameMismatch(ERS_HERE, ret.GetTypeName(), e.measurement());
  }

  from_entry(ret, e);
  
  return ret;
}
