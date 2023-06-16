

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

#include "../cut_helper.h"

using namespace ana;

bool useSliceCuts = false;
bool useSpillCuts = false;
bool onlyNuSlice = true;

std::vector<int> sliceCutIndices;

const SpillMultiVar kHITVARS([](const caf::SRSpillProxy* sr) -> std::vector<double> {
  std::vector<double> hits;
  int sliceID = 0;
  for (const auto& slc: sr->slc) {
    bool useSlice = true;
    if (useSliceCuts) { 
      for (const auto& i_cut : sliceCutIndices) { 
        const auto& cut = slice_cuts[i_cut];
        if (!cut(&slc)) { 
          useSlice = false; 
          break; 
        } 
      } 
    }
    if (onlyNuSlice) { if (!kSlcIsRecoNu(&slc)) {useSlice = false; } }
    if (!useSlice) { continue; }
    for (const auto& hit: slc.reco.hit) {
      hits.push_back(hit.spacepoint.XYZ.x);
      hits.push_back(hit.spacepoint.XYZ.y);
      hits.push_back(hit.spacepoint.XYZ.z);
      int cryo;
      if (hit.spacepoint.XYZ.x > 0) { cryo = 1; }
      else { cryo =-1; }
      hits.push_back(hit.spacepoint.pfpID*cryo);
      hits.push_back(sliceID);
      hits.push_back(hit.planeID);
    }
    sliceID++;
  }
  return hits;
});
const SpillMultiVar kCRTVARS([](const caf::SRSpillProxy* sr) -> std::vector<double> {
  std::vector<double> hits;
  for (const auto& hit : sr->crt_hits) {
    hits.push_back(hit.position.x);
    hits.push_back(hit.position.y);
    hits.push_back(hit.position.z);
    hits.push_back(hit.t1);
    hits.push_back(hit.plane);
  }
  return hits;
});
const SpillMultiVar kOPVARS([](const caf::SRSpillProxy* sr) -> std::vector<double> {
  std::vector<double> hits;
  for (const auto& hit : sr->opflashes) {
    //const int x_pos = hit.center.x; // cm
    //int x = -999;
    //if (x_pos > 200.215) { x = 200.215+2*74.745; }
    //else if (x_pos > 0) { x = 200.215-2*74.745; }
    //else if (x_pos > -200.215) { x = -200.215+2*74.745; }
    //else { x = -200.215-2*74.745; }
    const int x_pos = hit.cryo; // cm
    int x = 0;
    if (x_pos == 1) {x = 200.215+2*74.745;}
    else if (x_pos == 0) {x = -200.215-2*74.745;}
    hits.push_back(x);
    hits.push_back(hit.center.y);
    hits.push_back(hit.center.z);
    hits.push_back(hit.width.y);
    hits.push_back(hit.width.z);
    hits.push_back(hit.time);
  }
  return hits;
});
const SpillMultiVar kTRACKVARS([](const caf::SRSpillProxy* sr) -> std::vector<double> {
  std::vector<double> hits;
  int sliceID = 0;
  for (const auto& slc: sr->slc) {
    bool useSlice = true;
    if (useSliceCuts) { 
      for (const auto& i_cut : sliceCutIndices) { 
        const auto& cut = slice_cuts[i_cut];
        if (!cut(&slc)) { 
          useSlice = false; 
          break; 
        } 
      } 
    }
    if (onlyNuSlice) { if (!kSlcIsRecoNu(&slc)) {useSlice = false; } }
    if (!useSlice) { continue; }
    for (const auto& pfp : slc.reco.pfp) {
      if (pfp.trackScore > 0.5 && pfp.trackScore < 1.0 && !isnan(pfp.trk.start.x)) {
        hits.push_back(pfp.trk.start.x);
        hits.push_back(pfp.trk.start.y);
        hits.push_back(pfp.trk.start.z);
        hits.push_back(pfp.trk.end.x);
        hits.push_back(pfp.trk.end.y);
        hits.push_back(pfp.trk.end.z);
        int cryo;
        if (pfp.trk.start.x > 0) { cryo = 1; }
        else { cryo =-1; }
        hits.push_back(pfp.id*cryo);
        hits.push_back(sliceID);
      }
    }
    sliceID++; 
  }
  return hits;
});
const SpillMultiVar kSHOWERVARS([](const caf::SRSpillProxy* sr) -> std::vector<double> {
  std::vector<double> hits;
  int sliceID = 0;
  for (const auto& slc: sr->slc) {
    bool useSlice = true;
    if (useSliceCuts) { 
      for (const auto& i_cut : sliceCutIndices) { 
        const auto& cut = slice_cuts[i_cut];
        if (!cut(&slc)) { 
          useSlice = false; 
          break; 
        } 
      } 
    }
    if (onlyNuSlice) { if (!kSlcIsRecoNu(&slc)) {useSlice = false; } }
    if (!useSlice) { continue; }
    for (const auto& pfp : slc.reco.pfp) {
      if (pfp.trackScore <= 0.5 && pfp.trackScore > 0 && !isnan(pfp.shw.start.x)) {
        hits.push_back(pfp.shw.start.x);
        hits.push_back(pfp.shw.start.y);
        hits.push_back(pfp.shw.start.z);
        hits.push_back(pfp.shw.dir.x);
        hits.push_back(pfp.shw.dir.y);
        hits.push_back(pfp.shw.dir.z);
        hits.push_back(pfp.shw.len);
        hits.push_back(pfp.shw.open_angle);
        int cryo;
        if (pfp.shw.start.x > 0) { cryo = 1; }
        else { cryo =-1; }
        hits.push_back(pfp.id*cryo);
        hits.push_back(sliceID);
      }
    }
    sliceID++; 
  }
  return hits;
});
