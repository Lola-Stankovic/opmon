#ifndef DATA_RETRIEVAL_H_
#define DATA_RETRIEVAL_H_

#include <iostream>
#include <string>
#include <map>
#include <atomic>
#include <functional>
#include <any>
#include <mutex>  
#include <shared_mutex>
#include <thread>
#include <set>

class DataRetrieval
{
public:
  void setDataRetrieval(int port, const std::string& database_name,
                        const std::string& influxdb_uri);	 
  void retrieveValue(const std::string& metricName);
  void retrieveValueByHTTPRequest(const std::string& metricName);
  int getPort();
  const std::string& getDatabaseName();
  const std::string& getDatabaseHostAddress();

private:
   int port_;
   std::string database_name_;
   std::string influxdb_uri_;
  
};

#endif /* DATA_RETRIEVAL_H_ */
