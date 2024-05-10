/**
 * @file TriggerCandidateMakerChannelDistance.cpp
 *
 * This is part of the DUNE DAQ Application Framework, copyright 2021.
 * Licensing/copyright details are in the COPYING file that you should have
 * received with this code.
 */

#include "triggeralgs/ChannelDistance/TriggerCandidateMakerChannelDistance.hpp"

#include "TRACE/trace.h"
#define TRACE_NAME "TriggerCandidateMakerChannelDistancePlugin"

namespace triggeralgs {

void
TriggerCandidateMakerChannelDistance::set_new_tc(const TriggerActivity& input_ta)
{
  m_current_tc = TriggerCandidate();
  m_current_tc.inputs.push_back(input_ta);
  m_current_tp_count = input_ta.inputs.size();
  return;
}

void
TriggerCandidateMakerChannelDistance::configure(const nlohmann::json& config)
{
  if (config.contains("max_tp_count"))
    m_max_tp_count = config["max_tp_count"];
  return;
}

void
TriggerCandidateMakerChannelDistance::set_tc_attributes()
{
  auto& first_ta = m_current_tc.inputs.front();
  auto& last_ta = m_current_tc.inputs.back();

  m_current_tc.time_start = first_ta.time_start;
  m_current_tc.time_end = last_ta.time_end;
  m_current_tc.time_candidate = last_ta.time_start; // Since this is the TA that closed the TC.

  m_current_tc.detid = first_ta.detid;
  m_current_tc.algorithm = TriggerCandidate::Algorithm::kChannelDistance;
  m_current_tc.type = TriggerCandidate::Type::kChannelDistance;
  return;
}

void
TriggerCandidateMakerChannelDistance::operator()(const TriggerActivity& input_ta, std::vector<TriggerCandidate>& output_tcs)
{

  using namespace std::chrono;
  if (m_first_ta) {
    m_initial_offset = (duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count()) - (input_ta.time_start*(16*1e-6));
    m_first_ta = false;
  }

  // Update OpMon Variable(s)
  uint64_t system_time = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
  uint64_t data_time = input_ta.time_start*(16*1e-6);            
  m_data_vs_system_time_in.store(fabs(system_time - data_time - m_initial_offset));

  // Start a new TC if not already going.
  if (m_current_tc.inputs.empty()) {
    set_new_tc(input_ta);
    return;
  }

  // Check to close the TC based on TP contents.
  if (input_ta.inputs.size() + m_current_tp_count > m_max_tp_count) {
    set_tc_attributes();
    output_tcs.push_back(m_current_tc);

    // Update OpMon Variable(s)
    system_time = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
    data_time = m_current_tc.time_start*(16*1e-6);
    m_data_vs_system_time_out.store(fabs(system_time - data_time - m_initial_offset));

    set_new_tc(input_ta);
    return;
  }

  // Append the new TA and increase the TP count.
  m_current_tc.inputs.push_back(input_ta);
  m_current_tp_count += input_ta.inputs.size();
  return;
}

// Register algo in TC Factory
REGISTER_TRIGGER_CANDIDATE_MAKER(TRACE_NAME, TriggerCandidateMakerChannelDistance)

} // namespace triggeralgs
