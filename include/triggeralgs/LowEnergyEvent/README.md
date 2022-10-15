# Low Energy Event Trigger

 - Serves as introductory low energy event trigger algorithm template. Based on the same method of continuous scanning of time windows containing TPs.
 - In the past we have used only collection channel TPC hits to trigger on, for example, horizontal muons in the VD ColdBox runs at CERN. With the introduction of induction TPs used in this algorithm, it is hoped they can be leveraged for the identification and triggering on candidate low energy events.
 - The current approach to developing this algorithm is the simulation of TPC TPs in LArSoft, using Klaudia's branch of [duneana](https://github.com/DUNE/duneana/tree/feature/kwawrows_TriggerTPGen/duneana/DAQSimAna). The output TPs can be played through the `trigger`
[replay app](https://github.com/DUNE-DAQ/trigger/tree/develop/python/trigger/replay_tp_to_chain), which can be used to test the functionality of algorithms offline, in `dunedaq` code.

## Trigger Activity Maker
- We have 3 instances of the `Window` class, as used in the Horizontal Muon Algorithm (HMA). Each `Window` will contain the TPs received in a configurable amount of time from one of three TPC planes. Currently we first require an ADC spike above pure radiological background and noise hits (from simulation) which is the sum of ADC integrals of all windows. Second, this must be coupled with a short primary ionisation track in the collection plane, presumably caused by a Michel electron or SN event. The adjacency checking of the tried and tested Horizontal Muon Algorithm is used here.

## Trigger Candidate Maker
- At the moment, we use a trivial logic, by which we mean that there is a one-to-one map between emitted TAs and TCs. More complex triggering logic will be used in the future, when we want to cluster TAs from multiple detector elements (APAs).

 - For a talk on the related HMA, please see https://indico.fnal.gov/event/51502/contributions/226506/attachments/148509/190809/211019_FDDS_HorizontalMuon.pdf

