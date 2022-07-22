/**
 * @file Issues.hpp
 *
 * This is part of the DUNE DAQ Application Framework, copyright 2020.
 * Licensing/copyright details are in the COPYING file that you should have
 * received with this code.
 */

#ifndef OPMONLIB_INCLUDE_OPMONLIB_ISSUES_HPP_
#define OPMONLIB_INCLUDE_OPMONLIB_ISSUES_HPP_

#include <ers/Issue.hpp>
#include <string>

//namespace dunedaq {

//ERS_DECLARE_ISSUE(opmonlib, OpmonServiceCreationFailed, "OpmonServiceCreationFailed: " << error, ((std::string)error))

//} // namespace dunedaq

namespace dunedaq
{                                                                                                                                                                                                              
  namespace opmonlib{
    class OpmonServiceCreationFailed : public ers::Issue{
      template <class> friend class ers::IssueRegistrator;
    protected:
      OpmonServiceCreationFailed( const ers::Context & context ) : ers::Issue( context ) { ; }
    public:
      static const char * get_uid() {
	return "opmonlib::OpmonServiceCreationFailed";
      }
      OpmonServiceCreationFailed(const ers::Context & context, std::string error);
      OpmonServiceCreationFailed(const ers::Context & context, const std::string & msg , std::string error);
      OpmonServiceCreationFailed(const ers::Context & context, const std::string & msg , std::string error, const std::exception & cause );
      OpmonServiceCreationFailed( const ers::Context & context ,std::string error, const std::exception & cause );
      void raise() const {
	throw OpmonServiceCreationFailed(*this);
      }
      const char * get_class_name() const {
	return get_uid();
      }ers::Issue * clone() const {
	return new opmonlib::OpmonServiceCreationFailed(*this);
      } 
      std::string get_error() {
        std::string val;
        ers::Issue::get_value("error", val );
        return val;
      }
    };
  }

  namespace opmonlib
  {
    inline OpmonServiceCreationFailed::OpmonServiceCreationFailed
    (const ers::Context & context , std::string error ) : ers::Issue( context ) {
      set_value("error", error);
      std::ostringstream out; out << "OpmonServiceCreationFailed: " << error;
      prepend_message(out.str());
    }

    inline OpmonServiceCreationFailed::OpmonServiceCreationFailed
    (const ers::Context & context, const std::string & msg , std::string error ) : ers::Issue( context, msg ) { set_value("error", error); }
     
    inline OpmonServiceCreationFailed::OpmonServiceCreationFailed
    (const ers::Context & context, const std::string & msg , std::string error , const std::exception & cause ) : ers::Issue( context, msg , cause ) { set_value("error", error); }

    inline OpmonServiceCreationFailed::OpmonServiceCreationFailed
    (const ers::Context & context , std::string error, const std::exception & cause ) : ers::Issue( context , cause ) {set_value("error", error);

      std::ostringstream out;
      out << "OpmonServiceCreationFailed: " << error;
      prepend_message( out.str() ); } }

  namespace {

    ers::IssueRegistrator<opmonlib::OpmonServiceCreationFailed>
    opmonlib_OpmonServiceCreationFailed_instance;
  }
}


#endif // OPMONLIB_INCLUDE_OPMONLIB_ISSUES_HPP_


    
