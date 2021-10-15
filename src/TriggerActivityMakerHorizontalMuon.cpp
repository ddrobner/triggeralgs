/**
 * @file TriggerActivityMakerHorizontalMuon.cpp
 *
 * This is part of the DUNE DAQ Application Framework, copyright 2021.
 * Licensing/copyright details are in the COPYING file that you should have
 * received with this code.
 */

#include "triggeralgs/HorizontalMuon/TriggerActivityMakerHorizontalMuon.hpp"

#include "TRACE/trace.h"
#define TRACE_NAME "TriggerActivityMakerHorizontalMuon"

#include <vector>

using namespace triggeralgs;

void
TriggerActivityMakerHorizontalMuon::operator()(const TriggerPrimitive& input_tp, std::vector<TriggerActivity>& output_ta)
{
  
  // The first time operator is called, reset
  // window object.
  if(m_current_window.is_empty()){
    m_current_window.reset(input_tp);
    m_primitive_count++;
    return;
  } 

  // If the difference between the current TP's start time and the start of the window
  // is less than the specified window size, add the TP to the window.
  if((input_tp.time_start - m_current_window.time_start) < m_window_length){
    //TLOG_DEBUG(TRACE_NAME) << "Window not yet complete, adding the input_tp to the window.";
    m_current_window.add(input_tp);
  }
  // If the addition of the current TP to the window would make it longer
  // than the specified window length, don't add it but check whether the sum of all adc in
  // the existing window is above the specified threshold. If it is, and we are triggering on ADC,
  // make a TA and start a fresh window with the current TP.
  else if(m_current_window.adc_integral > m_adc_threshold && m_trigger_on_adc){
    //TLOG_DEBUG(TRACE_NAME) << "ADC integral in window is greater than specified threshold.";
    output_ta.push_back(construct_ta());
    //TLOG_DEBUG(TRACE_NAME) << "Resetting window with input_tp.";
    m_current_window.reset(input_tp);
  }
  // If the addition of the current TP to the window would make it longer
  // than the specified window length, don't add it but check whether the number of hit channels in
  // the existing window is above the specified threshold. If it is, and we are triggering on channels,
  // make a TA and start a fresh window with the current TP.
  else if(m_current_window.n_channels_hit() > m_adc_threshold && m_trigger_on_n_channels){
    //TLOG_DEBUG(TRACE_NAME) << "Number of channels hit in the window is greater than specified threshold.";
    output_ta.push_back(construct_ta());
    //TLOG_DEBUG(TRACE_NAME) << "Resetting window with input_tp.";
    m_current_window.reset(input_tp);
  }
  // If it is not, move the window along.
  else{
    //TLOG_DEBUG(TRACE_NAME) << "Window is at required length but specified threshold not met, shifting window along.";
    m_current_window.move(input_tp, m_window_length);
  }
  
  //TLOG_DEBUG(TRACE_NAME) << m_current_window;

  m_primitive_count++;

  return;
}

void
TriggerActivityMakerHorizontalMuon::configure(const nlohmann::json &config)
{
  //FIXME use some schema here
  if (config.is_object()){
    if (config.contains("trigger_on_adc")) m_trigger_on_adc = config["trigger_on_adc"];
    if (config.contains("trigger_on_n_channels")) m_trigger_on_n_channels = config["trigger_on_n_channels"];
    if (config.contains("adc_threshold")) m_adc_threshold = config["adc_threshold"];
    if (config.contains("n_channels_threshold")) m_n_channels_threshold = config["n_channels_threshold"];
    if (config.contains("window_length")) m_window_length = config["window_length"];
    //if (config.contains("channel_map")) m_channel_map = config["channel_map"];
  }
  else{
    TLOG_DEBUG(TRACE_NAME) << "The DEFAULT values of all configurables are being used.";
  }
  TLOG_DEBUG(TRACE_NAME) << "If the total ADC of trigger primitives with times within a "
                         << m_window_length << " tick time window is above " << m_adc_threshold << " counts, a trigger will be issued.";
}

TriggerActivity
TriggerActivityMakerHorizontalMuon::construct_ta() const
{
  TLOG_DEBUG(TRACE_NAME) << "I am constructing a trigger activity!";
  //TLOG_DEBUG(TRACE_NAME) << m_current_window;

  TriggerPrimitive latest_tp_in_window = m_current_window.tp_list.back();
  // The time_peak, time_activity, channel_* and adc_peak fields of this TA are irrelevent
  // for the purpose of this trigger alg.
  TriggerActivity ta{m_current_window.time_start, 
                     latest_tp_in_window.time_start+latest_tp_in_window.time_over_threshold,
                     latest_tp_in_window.time_peak,
                     latest_tp_in_window.time_peak,
                     latest_tp_in_window.channel,
                     latest_tp_in_window.channel,
                     latest_tp_in_window.channel,
                     m_current_window.adc_integral, 
                     latest_tp_in_window.adc_peak,
                     latest_tp_in_window.detid,
                     TriggerActivity::Type::kTPC,
                     TriggerActivity::Algorithm::kHorizontalMuon, 
                     0,
                     m_current_window.tp_list}; 
  return ta;
}

bool
TriggerActivityMakerHorizontalMuon::check_adjacency() const
{
  // FIX ME: An adjacency check on the channels which have hits. 
  return true;
}

