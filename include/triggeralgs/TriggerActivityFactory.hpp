/* @file: TriggerActivityFactory.hpp
 *
 * This is part of the DUNE DAQ Application Framework, copyright 2023.
 * Licensing/copyright details are in the COPYING file that you should have
 * received with this code.
 */

#ifndef TRIGGERALGS_TRIGGER_ACTIVITY_FACTORY_HPP_
#define TRIGGERALGS_TRIGGER_ACTIVITY_FACTORY_HPP_

#include "triggeralgs/TriggerActivityMaker.hpp"

#include <memory>
#include <string>
#include <unordered_map>
#include <functional>

#define REGISTER_TRIGGER_ACTIVITY_MAKER(tam_name, tam_class)                                                                                      \
  static struct tam_class##Registrar {                                                                                                            \
    tam_class##Registrar() {                                                                                                                      \
      TriggerActivityFactory::registerCreator(tam_name, []() -> std::shared_ptr<TriggerActivityMaker> {return std::make_shared<tam_class>();});   \
    }                                                                                                                                             \
  } tam_class##_registrar;

namespace triggeralgs {

class TriggerActivityFactory
{
  public:
    using TAMakerCreator = std::function<std::shared_ptr<TriggerActivityMaker>()>;
    using TAMakerMap = std::unordered_map<std::string, TAMakerCreator>;

  public:
    TriggerActivityFactory(const TriggerActivityFactory&) = delete;
    TriggerActivityFactory& operator=(const TriggerActivityFactory&) = delete;
    virtual ~TriggerActivityFactory() {}

    static std::shared_ptr<TriggerActivityMaker> makeTAMaker(const std::string& algName);

    static void registerCreator(const std::string algName, TAMakerCreator creator);

  private:
    static TAMakerMap& getTAMakers();
};

} /* namespace triggeralgs */

// TODO: Define ers exceptions
#endif // TRIGGERALGS_TRIGGER_ACTIVITY_FACTORY_HPP_
