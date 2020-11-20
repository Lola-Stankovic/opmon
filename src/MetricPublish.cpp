#include <iostream>
#include <string>
#include <map>
#include <mutex>
#include <shared_mutex>
#include <thread>
#include <unistd.h>
#include <future>
#include <set>
#include <chrono>
#include <cstdint>
#include <stdio.h>

#include "opmonlib/MetricPublish.hpp"

using namespace dunedaq::opmonlib;
using namespace std::chrono_literals;

uint64_t
MetricPublish::timeSinceEpochMillisec()
{
  using namespace std::chrono;
  return duration_cast<nanoseconds>(system_clock::now().time_since_epoch()).count();
}
