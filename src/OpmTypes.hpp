#ifndef OPMLIB_SRC_OPMTYPES_HPP_
#define OPMLIB_SRC_OPMTYPES_HPP_

#include <memory>
#include "MetricInterface.hpp"

namespace dunedaq {
namespace opmlib {

typedef std::shared_ptr<MetricRefInterface> MetricPtr;
//typedef std::map<std::string, MetricPtr> MetricSet;

//typedef std::set<std::string> StringSet;

}
}

#endif
