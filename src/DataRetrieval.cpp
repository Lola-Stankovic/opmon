#include <iostream>
#include <string>
#include <map>
#include <atomic>
#include <functional>
#include <any>
#include <mutex>  // For std::unique_lock
#include <shared_mutex>
#include <thread>
#include <set>

#include "DataRetrieval.hpp"
#include "influxdb.hpp"

/*
 * #include <stdio.h>
 * #include <curl/curl.h>
 * */

void
DataRetrieval::setDataRetrieval(int portNumber, const std::string& databaseName,
                                const std::string& influxdbUri)
{
  port_ = portNumber;
  database_name_ = databaseName;
  influxdb_uri_ = influxdbUri;   
}

void
DataRetrieval::retrieveValue(const std::string& metricName)
{
  influxdb_cpp::server_info si("127.0.0.1", getPort(), getDatabaseName());
  std::string resp;
  influxdb_cpp::query(resp, "select * from "+  metricName, si);
  std::cout<< resp<< "\n";; 
}
    
int
DataRetrieval::getPort()
{
  return port_;
}

const std::string&
DataRetrieval::getDatabaseName()
{
  return database_name_;
}

const std::string&
DataRetrieval::getDatabaseHostAddress()
{
return influxdb_uri_;
}

void
DataRetrieval::retrieveValueByHTTPRequest(const std::string& metricName)
{

}
