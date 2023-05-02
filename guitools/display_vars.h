

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

const SpillMultiVar kSLCVARS([](const caf::SRSpillProxy* sr) -> std::vector<double> {
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
const SpillMultiVar kSRVARS([](const caf::SRSpillProxy* sr) -> std::vector<double> {
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
