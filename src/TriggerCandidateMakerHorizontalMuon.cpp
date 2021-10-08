/**
 * @file TriggerCandidateMakerHorizontalMuon.cpp
 *
 * This is part of the DUNE DAQ Application Framework, copyright 2021.
 * Licensing/copyright details are in the COPYING file that you should have
 * received with this code.
 */

#include "triggeralgs/HorizontalMuon/TriggerCandidateMakerHorizontalMuon.hpp"

#include "TRACE/trace.h"
#define TRACE_NAME "TriggerCandidateMakerHorizontalMuon"

#include <vector>

using namespace triggeralgs;

void
TriggerCandidateMakerHorizontalMuon::operator()(const TriggerActivity& activity, std::vector<TriggerCandidate>& cand)
{ 

  // For now, if there is any single activity from any one detector element, emit
  // a trigger candidate.
  m_activity_count++;
  std::vector<detid_t> detid_vector = {activity.detid};
  std::vector<TriggerActivity> ta_list = {activity};

  TLOG_DEBUG(TRACE_NAME) << "Emitting an HorizontalMuon TriggerCandidate " << (m_activity_count-1);
  TriggerCandidate tc {
      activity.time_start, 
      activity.time_end,  
      activity.time_activity,
      detid_vector,
      TriggerCandidate::Type::kHorizontalMuon,
      TriggerCandidate::Algorithm::kHorizontalMuon,
      0,
      ta_list
  };
  cand.push_back(tc);

}

void
TriggerCandidateMakerHorizontalMuon::configure(const nlohmann::json &config)
{
}

