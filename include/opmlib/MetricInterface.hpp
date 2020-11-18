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

class MetricRefInterface
{
public:
  //virtual ~MetricRefInterface() = 0;
  virtual const std::string getTypeName()= 0;
};

template <typename T>
class MetricRef : public MetricRefInterface
{
public:
  MetricRef( T& ref) :	metric_ref_{ std::reference_wrapper<T>(ref)} 
  {
  }

  ~MetricRef() 
  {
  }

  std::reference_wrapper<T> getValue()
  {
    return metric_ref_;
  }

  const std::string getTypeName()
  {
    return std::string(typeid(T).name());
  }

private:
  std::reference_wrapper<T> metric_ref_;

};
