/**
 * @file TriggerCandidateMakerBundleN.cpp
 *
 * This is part of the DUNE DAQ Application Framework, copyright 2020.
 * Licensing/copyright details are in the COPYING file that you should have
 * received with this code.
 */

#include "triggeralgs/BundleN/TriggerCandidateMakerBundleN.hpp"

#include "TRACE/trace.h"
#define TRACE_NAME "TriggerCandidateMakerBundleNPlugin"

namespace triggeralgs {

bool TriggerCandidateMakerBundleN::bundle_condition() {
  return m_current_tc.inputs.size() == m_bundle_num;
}

void
TriggerCandidateMakerBundleN::operator()(const TriggerActivity& input_ta, std::vector<TriggerCandidate>& output_tcs)
{
  // Expect that TAs are inherently time ordered.
  m_current_tc.inputs.push_back(input_ta);

  if (bundle_condition()) {
    // Using the first TA as reference.
    dunedaq::trgdataformats::TriggerActivityData front_ta = m_current_tc.inputs.front();

    m_current_tc.time_start = front_ta.time_start;
    m_current_tc.time_end = m_current_tc.inputs.back().time_end;
    m_current_tc.time_candidate = front_ta.time_start; // TODO: Conforming. Do we change this?
    m_current_tc.detid = front_ta.detid;
    m_current_tc.type = TriggerCandidate::Type::kBundle;
    m_current_tc.algorithm = TriggerCandidate::Algorithm::kBundle;

    output_tcs.push_back(m_current_tc);

    // Reset the current.
    m_current_tc = TriggerCandidate();
  }

  // Should never reach this step. In this case, send it out.
  if (m_current_tc.inputs.size() > m_bundle_num) {
    TLOG(TLVL_DEBUG_1) << "Emitting large BundleN TriggerCandidate with " << m_current_tc.inputs.size() << " TAs.";

    // Using the first TA as reference.
    dunedaq::trgdataformats::TriggerActivityData front_ta = m_current_tc.inputs.front();

    TriggerCandidate tc;
    tc.time_start = front_ta.time_start;
    tc.time_end = m_current_tc.inputs.back().time_end;
    tc.time_candidate = front_ta.time_start; // TODO: Conforming. Do we change this?
    tc.detid = front_ta.detid;
    tc.type = TriggerCandidate::Type::kBundle; // TODO: Change to a meaningful type.
    tc.algorithm = TriggerCandidate::Algorithm::kBundle; // TODO: Make a kBundleN algo.

    output_tcs.push_back(tc);

    // Reset the current.
    m_current_tc = TriggerCandidate();
  }
}

void
TriggerCandidateMakerBundleN::configure(const nlohmann::json& config)
{
  if (config.is_object() && config.contains("bundle_number")) {
    m_bundle_num = config["bundle_number"];
  }
}

REGISTER_TRIGGER_CANDIDATE_MAKER(TRACE_NAME, TriggerCandidateMakerBundleN)
} // namespace triggeralgs

