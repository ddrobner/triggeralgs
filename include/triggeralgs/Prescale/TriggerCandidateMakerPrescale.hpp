/**
 * @file TriggerCandidateMakerPrescale.hpp
 *
 * This is part of the DUNE DAQ Application Framework, copyright 2020.
 * Licensing/copyright details are in the COPYING file that you should have
 * received with this code.
 */

#ifndef TRIGGERALGS_PRESCALE_TRIGGERCANDIDATEMAKERPRESCALE_HPP_
#define TRIGGERALGS_PRESCALE_TRIGGERCANDIDATEMAKERPRESCALE_HPP_

#include "triggeralgs/TriggerCandidateMaker.hpp"

#include <vector>

namespace triggeralgs {
class TriggerCandidateMakerPrescale : public TriggerCandidateMaker
{

public:
  /// The function that gets call when there is a new activity
  void operator()(const TriggerActivity&, std::vector<TriggerCandidate>&);

  void configure(const nlohmann::json& config);

private:
  uint64_t m_activity_count = 0; // NOLINT(build/unsigned)
  uint64_t m_prescale = 1;       // NOLINT(build/unsigned)
  timestamp_t m_readout_window_ticks_before =
    0; // How long before the input activity start time to make the candidate start time, in units of clock ticks
  timestamp_t m_readout_window_ticks_after =
    0; // How long after the input activity end time to make the candidate end time, in units of clock ticks
};

} // namespace triggeralgs

#endif // TRIGGERALGS_PRESCALE_TRIGGERCANDIDATEMAKERPRESCALE_HPP_
