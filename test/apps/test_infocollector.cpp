#include <nlohmann/json.hpp>
#include <iostream>
#include <chrono>
#include "ReusableThread.hpp"
#include "appfwk/NamedObject.hpp"

namespace dunedaq::mynamespace {

// This will be code-generated -------
struct MyInfo {
  public:
    // MyInfo();
    // ~MyInfo();

    static const std::string get_classname() { return "dunedaq::opmonlib::MyInfo"; }

    int32_t number;
    std::string something;
};

// This will be code-generated -------
struct MyInfoPlusPlus {
  public:
    // MyInfo();
    // ~MyInfo();

    static const std::string get_classname() { return "dunedaq::opmonlib::MyInfoPlusPlus"; }

    int32_t anotherNumber;
    std::string somethingElse;
};


inline void
to_json(nlohmann::json& j, const MyInfo& obj) {
    j["number"] = obj.number;
    j["something"] = obj.something;
}


inline void
to_json(nlohmann::json& j, const MyInfoPlusPlus& obj) {
    j["anotherNumber"] = obj.anotherNumber;
    j["somethingElse"] = obj.somethingElse;
}
}
// Code generation stops here ------

// Dark magic -- Don't look
namespace nlohmann {
template<typename Clock, typename Duration>
struct adl_serializer<std::chrono::time_point<Clock, Duration>> {
    static void to_json(json& j, const std::chrono::time_point<Clock, Duration>& tp) {
        j["since_epoch"] = std::chrono::duration_cast<std::chrono::microseconds>(tp.time_since_epoch()).count();
        j["unit"] = "microseconds";
    }
};
}
// Dark magic stops here -- Can look again

namespace dunedaq::opmonlib {
class InfoCollector {
  public:

    // Templated method to grab info blocks
    template<typename I>
    void add( I&& infoclass ) {
        infos[infoclass.get_classname()] = nlohmann::json{
            {"time", std::chrono::system_clock::now()}, //
            {"data", infoclass}
        };
    }

    // Puny getter
    const nlohmann::json& get_collected_infos() { return infos; }
  private:
    nlohmann::json infos;
};

}

namespace dunedaq::dummy {

class DummyModule : public appfwk::NamedObject {
  public:
    DummyModule(const std::string& name) : appfwk::NamedObject(name) {}
    virtual ~DummyModule() {}

    /* This is*/
    void get_info( opmonlib::InfoCollector& ic, int level = 0) {

        dunedaq::mynamespace::MyInfo my = { 15, "hello world"};
        ic.add(my);

        if ( level > 0 ) {
            dunedaq::mynamespace::MyInfoPlusPlus mypp = { 25, "hello world again"};
            ic.add(mypp);
        }

    }

};

class DummyModuleManager {
  public:
    DummyModuleManager() {};
    virtual ~DummyModuleManager() {};

    nlohmann::json gather_info(int level = 0) {

        nlohmann::json j;
        for ( auto& [n, m] : m_modulemap) {

            dunedaq::opmonlib::InfoCollector ic;
            m->get_info(ic, level);
            j[m->get_name()] = ic.get_collected_infos();
        }

        return j;
    }

    typedef std::map<std::string, std::shared_ptr<DummyModule>> DummyModuleMap_t; ///< DAQModules indexed by name
    DummyModuleMap_t m_modulemap;
};

}


int main(int /* argc */ , char const **/*argv[]*/) {
    /* code */

    // dunedaq::opmonlib::InfoCollector ic;

    // /// dentro DAQModule::get_info( InfoCollector& ic) {
    // dunedaq::mynamespace::MyInfo my = { 15, "hello world"};
    // dunedaq::mynamespace::MyInfoPlusPlus mypp = { 25, "hello world again"};

    // ic.add(my);
    // ic.add(mypp);
    // /// }

    // std::cout << ic.get_collected_infos().dump(4) << std::endl;
    // return 0;

    using namespace dunedaq;

    dummy::DummyModuleManager dmm;

    dmm.m_modulemap["a_module"] = std::make_shared<dummy::DummyModule>("a_module");
    dmm.m_modulemap["a_nother_module"] = std::make_shared<dummy::DummyModule>("a_nother_module");

    nlohmann::json j;
    j["app_id"] = "my_app";
    j["modules"] = dmm.gather_info(2);

    std::cout << j.dump(4) << std::endl;

}