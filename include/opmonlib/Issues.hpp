#ifndef OPMONLIB_INCLUDE_OPMONLIB_ISSUES_HPP_ 
#define OPMONLIB_INCLUDE_OPMONLIB_ISSUES_HPP_ 

#include <ers/ers.h>
#include <string>

namespace dunedaq {

ERS_DECLARE_ISSUE(opmonlib, UnsupportedUri,
                 "Unsupported URI: " << uri,
                 ((std::string)uri))

ERS_DECLARE_ISSUE(opmonlib, InternalError,
                 "Internal error: " << what,
                 ((std::string)what))
 
ERS_DECLARE_ISSUE(opmonlib, SetupPublisherError,
                 "SetupPublisherError: " << error,
                 ((std::string)error))

ERS_DECLARE_ISSUE(opmonlib, MetricPublishCreationFailed,
                 "MetricPublishCreationFailed: " << error,
                 ((std::string)error))

ERS_DECLARE_ISSUE(opmonlib, PublishingMetricsError,
                 "PublishingMetricsError: " << error,
                 ((std::string)error))

ERS_DECLARE_ISSUE(opmonlib, MetricRegistryError,
                 "MetricRegistryError: " << error,
                 ((std::string)error))

ERS_DECLARE_ISSUE(opmonlib, FilePublishError,
                 "FilePublishError: " << error,
                 ((std::string)error))

}

#endif // OPMONLIB_INCLUDE_OPMONLIB_ISSUES_HPP_   
