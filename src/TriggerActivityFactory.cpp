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

void TriggerActivityFactory::registerCreator(const std::string algName, TAMakerCreator creator)
{
  TAMakerMap& makers = getTAMakers();

  auto it = makers.find(algName);
  if (it == makers.end()) {
    makers[algName] = creator;
  }
}

std::shared_ptr<TriggerActivityMaker> TriggerActivityFactory::makeTAMaker(const std::string& algName)
{
  TAMakerMap& makers = getTAMakers();

  auto it = makers.find(algName);
  if (it != makers.end()) {
    return it->second();
  }

  return nullptr;
}
