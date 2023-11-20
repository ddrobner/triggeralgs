/* @file: TriggerActivityFactory.cpp
 *
 * This is part of the DUNE DAQ Application Framework, copyright 2023.
 * Licensing/copyright details are in the COPYING file that you should have
 * received with this code.
 */
#include <iostream>

#include "triggeralgs/TriggerActivityFactory.hpp"
#include "TRACE/trace.h"
#define TRACE_NAME "TriggerActivityFactory"

using namespace triggeralgs;

TriggerActivityFactory::TAMakerMap& TriggerActivityFactory::getTAMakers() {
  static TAMakerMap s_makers;
  return s_makers;
}

bool TriggerActivityFactory::registerCreator(const std::string name, TAMakerCreator creator)
{
  TAMakerMap& makers = getTAMakers();

  auto it = makers.find(name);
  if (it == makers.end()) {
    makers[name] = creator;
    std::cout << "Registered " << name << std::endl;
    return true;
  }

  std::cout << "Not registered.\n";
  return false;
}

std::shared_ptr<TriggerActivityMaker> TriggerActivityFactory::makeTAMaker(const std::string& algName)
{
  TAMakerMap& makers = getTAMakers();

  auto it = makers.find(algName);
  if (it != makers.end()) {
    std::cout << "Found " << name << "." << std::endl;
    return it->second();
  }

  std::cout << "Requested " << name << " not found.\n";
  return nullptr;
}
