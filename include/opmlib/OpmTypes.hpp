#ifndef OPMONLIB_SRC_OPMTYPES_HPP_
#define OPMONLIB_SRC_OPMTYPES_HPP_

#include <memory>
#include "MetricInterface.hpp"

namespace dunedaq {
namespace opmonlib {

typedef std::shared_ptr<MetricRefInterface> MetricPtr;

//typedef std::map<std::string, MetricPtr> MetricSet;

//typedef std::set<std::string> StringSet;

}
}

#endif
