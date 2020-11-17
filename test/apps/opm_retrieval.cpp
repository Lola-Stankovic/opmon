#include <iostream>
#include <string>
#include <atomic>
#include <mutex>  // For std::unique_lock
#include <shared_mutex>
#include <chrono>
#include <future>
#include <thread>

#include "DataRetrieval.hpp"

int main(int /*argc*/, char** /*argv*/)
{ 
  DataRetrieval dretrieval;
  dretrieval.setDataRetrieval(8086, "prometheus_lola", "localhost");
  dretrieval.retrieveValue("Humidity");                                                                    
}                                                                    
