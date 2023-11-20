/* @file: TriggerActivityFactory.hpp
 *
 * This is part of the DUNE DAQ Application Framework, copyright 2023.
 * Licensing/copyright details are in the COPYING file that you should have
 * received with this code.
 */

#ifndef TRIGGERALGS_TRIGGER_ACTIVITY_FACTORY_HPP_
#define TRIGGERALGS_TRIGGER_ACTIVITY_FACTORY_HPP_

#include <memory>
#include <string>
#include <unordered_map>

namespace triggeralgs {
class TriggerActivityFactory
{
  public:
    using TAMakerCreator = shared_ptr<TriggerActivityMaker>(*)();
    using TAMakerMap = std::unordered_map<std::string, TAMakerCreator>;

  public:
    TriggerActivityFactory(const TriggerActivityFactory&) = delete;
    TriggerActivityFactory& operator=(const TriggerActivityFactory&) = delete;
    virtual ~TriggerActivityFactory() {}

    static shared_ptr<TriggerActivityMaker> makeTAMaker(const std::string& algName);

    static bool registerCreator(const std::string algName, TAMakerCreator creator);

  private:
    static CreatorMap& getTAMakers();
};

// TODO: Define ers exceptions
#endif // TRIGGERALGS_TRIGGER_ACTIVITY_FACTORY_HPP_
