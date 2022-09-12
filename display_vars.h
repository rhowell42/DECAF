

#include "sbnana/CAFAna/Core/Binning.h"
#include "sbnana/CAFAna/Core/Cut.h"
//#include "sbnana/CAFAna/StandardRecord/Proxy/SRProxy.h"
#include "sbnanaobj/StandardRecord/Proxy/SRProxy.h"

#include "sbnana/SBNAna/Vars/NumuVars.h"
#include "sbnana/SBNAna/Vars/NueVars.h"
#include "sbnana/SBNAna/Vars/Vars.h"
#include "sbnana/SBNAna/Vars/TruthVars.h"
#include "sbnana/SBNAna/Cuts/Cuts.h"
#include "sbnana/SBNAna/Cuts/TruthCuts.h"
#include "sbnana/SBNAna/Cuts/NueCuts.h"

#include "TVector3.h"
#include "TLorentzVector.h"
#include "TCanvas.h"
#include "TH1.h"
#include "TLegend.h"
#include "TMath.h"

#include "cut_helper.h"

using namespace ana;

bool useSliceCuts = false;
bool useSpillCuts = false;
bool onlyNuSlice = true;

const SpillMultiVar kTPCX([](const caf::SRSpillProxy* sr) -> std::vector<double> {
  std::vector<double> hits;
  for (const auto& slc: sr->slc) {
    bool useSlice = true;
    if (useSliceCuts) { for (const auto& cut : slice_cuts) { if (!cut(&slc)) { useSlice = false; break; } } }
    if (onlyNuSlice) { if (!kSlcIsRecoNu(&slc)) {useSlice = false; } }
    if (!useSlice) { continue; }
    for (const auto& hit: slc.reco.hit) {
      hits.push_back(hit.spacepoint.XYZ.x);
    }
  }
  return hits;
});
const SpillMultiVar kTPCY([](const caf::SRSpillProxy* sr) -> std::vector<double> {
  std::vector<double> hits;
  for (const auto& slc: sr->slc) {
    bool useSlice = true;
    if (useSliceCuts) { for (const auto& cut : slice_cuts) { if (!cut(&slc)) { useSlice = false; break; } } }
    if (onlyNuSlice) { if (!kSlcIsRecoNu(&slc)) {useSlice = false; } }
    if (!useSlice) { continue; }
    for (const auto& hit: slc.reco.hit) {
      hits.push_back(hit.spacepoint.XYZ.y);
    }
  }
  return hits;
});
const SpillMultiVar kTPCZ([](const caf::SRSpillProxy* sr) -> std::vector<double> {
  std::vector<double> hits;
  for (const auto& slc: sr->slc) {
    bool useSlice = true;
    if (useSliceCuts) { for (const auto& cut : slice_cuts) { if (!cut(&slc)) { useSlice = false; break; } } }
    if (onlyNuSlice) { if (!kSlcIsRecoNu(&slc)) {useSlice = false; } }
    if (!useSlice) { continue; }
    for (const auto& hit: slc.reco.hit) {
      hits.push_back(hit.spacepoint.XYZ.z);
    }
  }
  return hits;
});
const SpillMultiVar kPFPID([](const caf::SRSpillProxy* sr) -> std::vector<double> {
  std::vector<double> hits;
  for (const auto& slc: sr->slc) {
    bool useSlice = true;
    if (useSliceCuts) { for (const auto& cut : slice_cuts) { if (!cut(&slc)) { useSlice = false; break; } } }
    if (onlyNuSlice) { if (!kSlcIsRecoNu(&slc)) {useSlice = false; } }
    if (!useSlice) { continue; }
    for (const auto& hit: slc.reco.hit) {
      hits.push_back(hit.spacepoint.pfpID);
    }
  }
  return hits;
});
const SpillMultiVar kSliceID([](const caf::SRSpillProxy* sr) -> std::vector<double> {
  std::vector<double> hits;
  int sliceID = 0;
  for (const auto& slc: sr->slc) {
    bool useSlice = true;
    if (useSliceCuts) { for (const auto& cut : slice_cuts) { if (!cut(&slc)) { useSlice = false; break; } } }
    if (onlyNuSlice) { if (!kSlcIsRecoNu(&slc)) {useSlice = false; } }
    if (!useSlice) { continue; }
    for (const auto& hit: slc.reco.hit) {
      hits.push_back(sliceID);
    }
    sliceID++;
  }
  return hits;
});

