/**
 * @file InfoProvider.hpp
 *
 * This is part of the DUNE DAQ Application Framework, copyright 2020.
 * Licensing/copyright details are in the COPYING file that you should have
 * received with this code.
 */

#ifndef OPMONLIB_INCLUDE_OPMONLIB_INFOPROVIDER_HPP_
#define OPMONLIB_INCLUDE_OPMONLIB_INFOPROVIDER_HPP_

#include "opmonlib/InfoCollector.hpp"
#include <iostream>

namespace dunedaq::opmonlib {

class InfoProvider
{

public:
  virtual void gather_stats(opmonlib::InfoCollector& ic, int level) = 0;

private:
};

} // namespace dunedaq::opmonlib

#endif // OPMONLIB_INCLUDE_OPMONLIB_INFOPROVIDER_HPP_
