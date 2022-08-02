/**
 * @file TriggerActivityMakerLowEnergyEvent.hpp
 *
 * This is part of the DUNE DAQ Application Framework, copyright 2021.
 * Licensing/copyright details are in the COPYING file that you should have
 * received with this code.
 */

#ifndef TRIGGERALGS_LOWENERGYEVENT_TRIGGERACTIVITYMAKERLOWENERGYEVENT_HPP_
#define TRIGGERALGS_LOWENERGYEVENT_TRIGGERACTIVITYMAKERLOWENERGYEVENT_HPP_

#include "detchannelmaps/TPCChannelMap.hpp"
#include "triggeralgs/TriggerActivityMaker.hpp"
#include <fstream>
#include <vector>

namespace triggeralgs {
class TriggerActivityMakerLowEnergyEvent : public TriggerActivityMaker
{

public:
  void operator()(const TriggerPrimitive& input_tp, std::vector<TriggerActivity>& output_ta);

  void configure(const nlohmann::json& config);

private:
  class Window
  {
  public:
    bool is_empty() const { return inputs.empty(); };
    void add(TriggerPrimitive const& input_tp)
    {
      // Add the input TP's contribution to the total ADC, increase hit 
      // channel's hit count and add it to the TP list.
      adc_integral += input_tp.adc_integral;
      channel_states[input_tp.channel]++;
      inputs.push_back(input_tp);
    };
    void clear() { inputs.clear(); };
    uint16_t n_channels_hit() { return channel_states.size(); };
    void move(TriggerPrimitive const& input_tp, timestamp_t const& window_length)
    {
      // Find all of the TPs in the window that need to be removed
      // if the input_tp is to be added and the size of the window
      // is to be conserved.
      // Substract those TPs' contribution from the total window ADC and remove their
      // contributions to the hit counts.
      uint32_t n_tps_to_erase = 0;
      for (auto tp : inputs) {
        if (!(input_tp.time_start - tp.time_start < window_length)) {
          n_tps_to_erase++;
          adc_integral -= tp.adc_integral;
          channel_states[tp.channel]--;
          // If a TP being removed from the window results in a channel no longer having
          // any hits, remove from the states map so map.size() can be used for number
          // channels hit.
          if (channel_states[tp.channel] == 0)
            channel_states.erase(tp.channel);
        } else
          break;
      }
      // Erase the TPs from the window.
      inputs.erase(inputs.begin(), inputs.begin() + n_tps_to_erase);
      // Make the window start time the start time of what is now the first TP.

      if (!(inputs.size() == 0)) {
        time_start = inputs.front().time_start;
        add(input_tp);
      } else {
        reset(input_tp);
      }
    };
    void reset(TriggerPrimitive const& input_tp)
    {

      // Empty the channel and TP lists.
      channel_states.clear();
      inputs.clear();
      // Set the start time of the window to be the start time of theinput_tp.
      time_start = input_tp.time_start;
      // Start the total ADC integral.
      adc_integral = input_tp.adc_integral;
      // Start hit count for the hit channel.
      channel_states[input_tp.channel]++;
      // Add the input TP to the TP list.
      inputs.push_back(input_tp);
      // std::cout << "Number of channels hit: " << n_channels_hit() << std::endl;
    };
    friend std::ostream& operator<<(std::ostream& os, const Window& window)
    {
      if (window.is_empty()) {os << "Window is empty!\n";}
      else {
        os << "Window start: " << window.time_start << ", end: " << window.inputs.back().time_start;
        os << ". Total of: " << window.adc_integral << " ADC counts with " << window.inputs.size() << " TPs.\n";
        os << window.channel_states.size() << " independent channels have hits.\n";
      }
      return os;
    };

    timestamp_t time_start;
    uint32_t adc_integral;
    std::unordered_map<channel_t, uint16_t> channel_states;
    std::vector<TriggerPrimitive> inputs;
  };

  TriggerActivity construct_ta(Window m_current_window) const;
  uint16_t check_adjacency(Window window) const; // Returns longest string of adjacent collection hits in window

  Window m_current_window;
  uint64_t m_primitive_count = 0;
  int check_tot(Window m_current_window) const;
  
  // Make 3 extra instances of the embedded Window class. One for each view.
  Window m_collection_window;
  Window m_induction_window;
  // Window m_induction2_window();

  // Configurable parameters.
  bool m_trigger_on_adc = true;
  bool m_trigger_on_n_channels = true;
  bool m_trigger_on_adjacency = true;   // Default use of the triggering
  uint16_t m_adjacency_threshold = 15;   // Default is 15 wire track for testing
  int m_max_adjacency = 0;               // The maximum adjacency seen so far in any window
  uint32_t m_tot_threshold = 2000;       // Work out good values for this
  uint32_t m_adc_threshold = 300000;    // Not currently triggering on this
  uint16_t m_n_channels_threshold = 20;  // Set this to ~80 for frames.bin, ~150-300 for tps_link_11.txt
  uint16_t m_adj_tolerance = 5;          // Adjacency tolerance - default is 3 from coldbox testing.
  int index = 0;
  uint16_t ta_adc = 0;
  uint16_t ta_channels = 0;
  timestamp_t m_window_length = 3000;    // Shouldn't exceed the max drift

  // For debugging purposes.
  void add_window_to_record(Window window);
  void dump_window_record(Window window);
  void dump_tp(TriggerPrimitive const& input_tp);
  std::vector<Window> m_window_record;
};
} // namespace triggeralgs

#endif // TRIGGERALGS_LOWENERGYEVENT_TRIGGERACTIVITYMAKERLOWENERGYEVENT_HPP_
