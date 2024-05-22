/**
 * @file TriggerActivityMaker.hpp
 *
 * This is part of the DUNE DAQ Application Framework, copyright 2020.
 * Licensing/copyright details are in the COPYING file that you should have
 * received with this code.
 */

#ifndef TRIGGERALGS_INCLUDE_TRIGGERALGS_TRIGGERACTIVITYMAKER_HPP_
#define TRIGGERALGS_INCLUDE_TRIGGERALGS_TRIGGERACTIVITYMAKER_HPP_

#include "triggeralgs/Issues.hpp"
#include "triggeralgs/Logging.hpp"
#include "triggeralgs/TriggerActivity.hpp"
#include "triggeralgs/TriggerPrimitive.hpp"
#include "triggeralgs/Types.hpp"

#include <nlohmann/json.hpp>
#include <vector>
#include <atomic>
#include <chrono>

namespace triggeralgs {

class TriggerActivityMaker
{
public:
  virtual ~TriggerActivityMaker() = default;
  virtual void operator()(const TriggerPrimitive& input_tp, std::vector<TriggerActivity>& output_ta) = 0;
  virtual void flush(timestamp_t /* until */, std::vector<TriggerActivity>&) {}
  virtual void configure(const nlohmann::json&) {}

  std::atomic<uint64_t> m_data_vs_system_time_in  = 0;
  std::atomic<uint64_t> m_data_vs_system_time_out = 0;
  std::atomic<uint64_t> m_initial_offset = 0;
  std::atomic<bool>     m_first_tp = true;
  std::atomic<bool>     m_use_latency_offset;
  // to convert 62.5MHz clock ticks to ms: 1/62500000 = 0.000000016 <- seconds per tick; 0.000016 <- ms per tick; 16*1e-6 <- sci notation
  std::atomic<double>   m_clock_ticks_to_ms = 16*1e-6;

  virtual void use_latency(const bool use) { m_use_latency_offset = use; }
};

} // namespace triggeralgs

#endif // TRIGGERALGS_INCLUDE_TRIGGERALGS_TRIGGERACTIVITYMAKER_HPP_
