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
  port=portNumber;
  database_name=databaseName;
  influxdb_uri=influxdbUri;   
}

void
DataRetrieval::retrieveValue(const std::string& metricName)
{
  influxdb_cpp::server_info si("127.0.0.1", getPort(), getDatabaseName());
  std::string resp;
  influxdb_cpp::query(resp, "select * from "+  metricName, si);
  std::cout<< resp<< "\n";; 
}
    
//curl -G 'http://localhost:8086/query?pretty=true' --data-urlencode "db=prometheus_lola" --data-urlencode "q=SELECT value FROM cpu_load_short"

const int
DataRetrieval::getPort()
{
  return port;
}

const std::string&
DataRetrieval::getDatabaseName()
{
  return database_name;
}

const std::string&
DataRetrieval::getDatabaseHostAddress()
{
return influxdb_uri;
}

void
DataRetrieval::retrieveValueByHTTPRequest(const std::string& metricName)
{

}
