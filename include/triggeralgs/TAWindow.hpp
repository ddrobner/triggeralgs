/* @file: TAWindow.hpp
 *
 * This is party of the DUNE DAQ Application Framework, copyright 2021.
 * Licensing/copyright details are in the COPYING file that you should have
 * received with this code.
 */

#ifndef TRIGGERALGS_TAWINDOW_HPP_
#define TRIGGERALGS_TAWINDOW_HPP_

#include <fstream>
#include <vector>

#include <chrono>
#include <map>
#include <memory>
#include <string>
#include <unordered_set>
#include <utility>

namespace triggeralgs {

  class TAWindow
  {
  public:
    
    bool is_empty() const { return inputs.empty(); };

    void add(const TriggerActivity& input_ta)
    {
      // Add the input TA's contribution to the total ADC, increase the hit count
      // of all of the channels which feature and add it to the TA list keeping the TA
      // list time ordered by time_start. Preserving time order makes moving easier.
      adc_integral += input_ta.adc_integral;
      for (TriggerPrimitive tp : input_ta.inputs) {
        channel_states[tp.channel]++;
      }
      // Perform binary search based on time_start.
      uint16_t insert_at = 0;
      for (auto ta : inputs) {
        if (input_ta.time_start < ta.time_start)
          break;
        insert_at++;
      }
      inputs.insert(inputs.begin() + insert_at, input_ta);
    };

    void clear() { inputs.clear(); };

    uint16_t n_channels_hit() { return channel_states.size(); };

    void move(TriggerActivity const& input_ta, timestamp_t const& window_length)
    {
      // Find all of the TAs in the window that need to be removed
      // if the input_ta is to be added and the size of the window
      // is to be conserved.
      // Subtract those TAs' contribution from the total window ADC and remove their
      // contributions to the hit counts.
      uint32_t n_tas_to_erase = 0;
      for (auto ta : inputs) {
        if (!(input_ta.time_start - ta.time_start < window_length)) {
          n_tas_to_erase++;
          adc_integral -= ta.adc_integral;
          for (TriggerPrimitive tp : ta.inputs) {
            channel_states[tp.channel]--;
            // If a TA being removed from the window results in a channel no longer having
            // any hits, remove from the states map so map.size() can be used for number
            // channel hits.
            if (channel_states[tp.channel] == 0)
              channel_states.erase(tp.channel);
          }
        } else
          break;
      }
      // Erase the TAs from the window.
      inputs.erase(inputs.begin(), inputs.begin() + n_tas_to_erase);
      // Make the window start time the start time of what is now the
      // first TA.
      if (inputs.size() != 0) {
        add(input_ta);
      } else {
        add(input_ta);
      }
      time_start = inputs.front().time_start;

    }

    void reset(TriggerActivity const& input_ta)
    {
      // Empty the channel and TA lists.
      channel_states.clear();
      inputs.clear();
      // Set the start time of the window to be the start time of the
      // input_ta.
      time_start = input_ta.time_start;
      // Start the total ADC integral.
      adc_integral = input_ta.adc_integral;
      // Start hit count for the hit channels.
      for (TriggerPrimitive tp : input_ta.inputs) {
        channel_states[tp.channel]++;
      }
      // Add the input TA to the TA list.
      inputs.push_back(input_ta);
    }

    friend std::ostream& operator<<(std::ostream& os, const TAWindow& window)
    {
      if (window.is_empty())
        os << "Window is empty!\n";
      else{
        os << "Window start: " << window.time_start << ", end: " << window.inputs.back().time_start;
        os << ". Total of: " << window.adc_integral << " ADC counts with " << window.inputs.size() << " TPs.\n";
        os << window.channel_states.size() << " independent channels have hits.\n";
      }
      return os;
    };

    timestamp_t time_start;
    uint64_t adc_integral;
    std::unordered_map<channel_t, uint16_t> channel_states;
    std::vector<TriggerActivity> inputs;
  };

} // namespace triggeralgs

#endif // TRIGGERALGS_TAWINDOW_HPP_
