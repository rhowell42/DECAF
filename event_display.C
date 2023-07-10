#define GET_VARIABLE_NAME(Variable)(#Variable)

#include "sbnana/CAFAna/Core/SpectrumLoader.h"
#include "sbnana/CAFAna/Core/Spectrum.h"
#include "sbnana/CAFAna/Core/Binning.h"
#include "guitools/display_vars.h"
#include "guitools/GUI.h"
#include "guitools/MultiView.h"
#include "tools/plot_tools.h"
#include "TFile.h"
#include "TStyle.h"
#include "TEveViewer.h"
#include "TEveManager.h"
#include "TEvePointSet.h"
#include "TGLRnrCtx.h"
#include "TGeoManager.h"
#include "TEveGeoShape.h"
#include "TEveBox.h"
#include "TGeoVolume.h"
#include "TGeoMedium.h"
#include "TEveGeoNode.h"
#include "TEveLine.h"
#include "TEveBoxSet.h"
#include <TEveManager.h>
#include <TEveViewer.h>
#include <TGLViewer.h>
#include <TEveScene.h>
#include <TEveProjectionManager.h>
#include <TEveProjectionAxes.h>
#include <TEveEventManager.h>
#include <TEveBrowser.h>
#include <TEveWindow.h>
#include "TMarker3DBox.h"

using namespace ana;

const SpillVar kFindEvents([](const caf::SRSpillProxy *sr)->int {
  hitvars = kHITVARS(sr);
  crtvars = kCRTVARS(sr);
  opvars = kOPVARS(sr);
  trackvars = kTRACKVARS(sr);
  showervars = kSHOWERVARS(sr);
  detector = kDETECTOR(sr);

  std::vector<double>xarray;
  std::vector<double>yarray;
  std::vector<double>zarray;
  std::vector<double>pfparray;
  std::vector<double>slicearray;
  std::vector<double>planeidarray;

  std::vector<double>crtxarray;
  std::vector<double>crtyarray;
  std::vector<double>crtzarray;
  std::vector<double>crttimearray;
  std::vector<double>crtplanearray;

  std::vector<double>flashxarray;
  std::vector<double>flashyarray;
  std::vector<double>flashzarray;
  std::vector<double>flashwidthyarray;
  std::vector<double>flashwidthzarray;
  std::vector<double>flashtimearray;

  std::vector<double>trackStartXarray;
  std::vector<double>trackStartYarray;
  std::vector<double>trackStartZarray;
  std::vector<double>trackEndXarray;
  std::vector<double>trackEndYarray;
  std::vector<double>trackEndZarray;
  std::vector<double>trackPFParray;
  std::vector<double>trackSlicearray;

  std::vector<double>showerStartXarray;
  std::vector<double>showerStartYarray;
  std::vector<double>showerStartZarray;
  std::vector<double>showerDirXarray;
  std::vector<double>showerDirYarray;
  std::vector<double>showerDirZarray;
  std::vector<double>showerLengtharray;
  std::vector<double>showerAnglearray;
  std::vector<double>showerPFParray;
  std::vector<double>showerSlicearray;

  if (!hitvars.empty()) {
    for (size_t i = 0; i < hitvars.size() - kHitEnd; i += kHitEnd) {
      xarray.push_back(hitvars[i + x]);
      yarray.push_back(hitvars[i + y]);
      zarray.push_back(hitvars[i + z]);
      pfparray.push_back(hitvars[i + kpfp]);
      slicearray.push_back(hitvars[i + kslice]);
      planeidarray.push_back(hitvars[i + kplaneid]);
    }

    X.push_back(xarray);
    Y.push_back(yarray);
    Z.push_back(zarray);
    PFPID.push_back(pfparray);
    SliceID.push_back(slicearray);
    PlaneID.push_back(planeidarray);
  }
  if (!crtvars.empty()) {
  for (size_t i = 0; i < crtvars.size() - kCRTEnd; i += kCRTEnd) {
    crtxarray.push_back(crtvars[i + crtx]);
    crtyarray.push_back(crtvars[i + crty]);
    crtzarray.push_back(crtvars[i + crtz]);
    crttimearray.push_back(crtvars[i + crttime]);
    crtplanearray.push_back(crtvars[i + crtplane]);
  }

  CRTX.push_back(crtxarray);
  CRTY.push_back(crtyarray);
  CRTZ.push_back(crtzarray);
  CRTTime.push_back(crttimearray);
  CRTPlane.push_back(crtplanearray);
  }

  if (!opvars.empty()) {
  for (size_t i = 0; i < opvars.size() - kFlashEnd; i += kFlashEnd) {
    flashxarray.push_back(opvars[i + flashx]);
    flashyarray.push_back(opvars[i + flashy]);
    flashzarray.push_back(opvars[i + flashz]);
    flashtimearray.push_back(opvars[i + flashtime]);
    flashwidthyarray.push_back(opvars[i + flashwidthy]);
    flashwidthzarray.push_back(opvars[i + flashwidthz]);
  }

  FlashX.push_back(flashxarray);
  FlashY.push_back(flashyarray);
  FlashZ.push_back(flashzarray);
  FlashTime.push_back(flashtimearray);
  FlashWidthY.push_back(flashwidthyarray);
  FlashWidthZ.push_back(flashwidthzarray);
  }

  if (!trackvars.empty()) {
  for (size_t i = 0; i < trackvars.size() - kTrackEnd; i += kTrackEnd) {
    trackStartXarray.push_back(trackvars[i + trackstartx]);
    trackStartYarray.push_back(trackvars[i + trackstarty]);
    trackStartZarray.push_back(trackvars[i + trackstartz]);
    trackEndXarray.push_back(trackvars[i + trackendx]);
    trackEndYarray.push_back(trackvars[i + trackendy]);
    trackEndZarray.push_back(trackvars[i + trackendz]);
    trackPFParray.push_back(trackvars[i + trackpfp]);
    trackSlicearray.push_back(trackvars[i + trackslice]);
  }

  TrackStartX.push_back(trackStartXarray);
  TrackStartY.push_back(trackStartYarray);
  TrackStartZ.push_back(trackStartZarray);
  TrackEndX.push_back(trackEndXarray);
  TrackEndY.push_back(trackEndYarray);
  TrackEndZ.push_back(trackEndZarray);
  TrackPFP.push_back(trackPFParray);
  TrackSlice.push_back(trackSlicearray);
  }

  if (!showervars.empty()) {
  for (size_t i = 0; i < showervars.size() - kShowerEnd; i += kShowerEnd) {
    showerStartXarray.push_back(showervars[i + showerstartx]);
    showerStartYarray.push_back(showervars[i + showerstarty]);
    showerStartZarray.push_back(showervars[i + showerstartz]);
    showerDirXarray.push_back(showervars[i + showerdirx]);
    showerDirYarray.push_back(showervars[i + showerdiry]);
    showerDirZarray.push_back(showervars[i + showerdirz]);
    showerLengtharray.push_back(showervars[i + showerlength]);
    showerAnglearray.push_back(showervars[i + showerangle]);
    showerPFParray.push_back(showervars[i + showerpfp]);
    showerSlicearray.push_back(showervars[i + showerslice]);
  }

  ShowerStartX.push_back(showerStartXarray);
  ShowerStartY.push_back(showerStartYarray);
  ShowerStartZ.push_back(showerStartZarray);
  ShowerDirX.push_back(showerDirXarray);
  ShowerDirY.push_back(showerDirYarray);
  ShowerDirZ.push_back(showerDirZarray);
  ShowerLength.push_back(showerLengtharray);
  ShowerAngle.push_back(showerAnglearray);
  ShowerPFP.push_back(showerPFParray);
  ShowerSlice.push_back(showerSlicearray);
  }

  run.push_back(kRun(sr));
  event.push_back(kEvt(sr));

  nSpills++;
  return 42;
});

void LoadReco() {
  if (event.empty()) {
    gMultiView->DestroyEventRPhi();

    gMultiView->DestroyEventRhoZ();
    gEve->Redraw3D(kFALSE, kTRUE);

    return;
  }

  auto top = gEve->GetCurrentEvent();
  std::list < TEveElement *> children;
  top->FindChildren(children, "Reco");
  for (auto
    const & child: children) {
    if (child) {
      child->Destroy();
    }
  }

  if (!TrackStartX.empty()) {
  int c = 0;
  for (size_t e = 0; e < TrackStartX[spill].size(); e++) {
    if (c == 10 || c == 19) {
      c++;
    }

    auto line = new TEveLine;
    line->SetNextPoint(TrackStartX[spill][e], TrackStartY[spill][e], TrackStartZ[spill][e]);
    line->SetNextPoint(TrackEndX[spill][e], TrackEndY[spill][e], TrackEndZ[spill][e]);
    if (ColorBySlice) {
      line->SetMainColor(TrackSlice[spill][e]);
    } else if (ColorByPFPs) {
      line->SetMainColor(abs(TrackPFP[spill][e]));
    }
    line->SetMainTransparency(25);
    line->SetLineWidth(5);
    line->SetElementName("Reco");

    gEve->AddElement(line);
  }
    top = gEve->GetCurrentEvent();

    gMultiView->DestroyEventRPhi();
    gMultiView->ImportEventRPhi(top);

    gMultiView->DestroyEventRhoZ();
    gMultiView->ImportEventRhoZ(top);
    gEve->Redraw3D(kFALSE, kTRUE);
    c++;
  }
  TEveBoxSet *showers = new TEveBoxSet;
  showers->Reset(TEveBoxSet::kBT_Cone, kFALSE, 64);
  TEveVector pos, dir;
  if (!ShowerStartX.empty()) {
  for (size_t e = 0; e < ShowerStartX[spill].size(); e++) {

    pos.Set(ShowerStartX[spill][e], ShowerStartY[spill][e], ShowerStartZ[spill][e]);
    dir.Set(ShowerDirX[spill][e], ShowerDirY[spill][e], ShowerDirZ[spill][e]);
    dir *= ShowerLength[spill][e];
    double radius = TMath::Tan(ShowerAngle[spill][e]) *ShowerLength[spill][e];

    showers->AddCone(pos, dir, radius);
    showers->DigitValue(ShowerPFP[spill][e]);
    if (ColorBySlice) {
      showers->DigitColor(ShowerSlice[spill][e], 25);
    } else if (ColorByPFPs) {
      showers->DigitColor(abs(ShowerPFP[spill][e]), 25);
    }
  }
  showers->SetElementName("Reco");
  showers->RefitPlex();
  gEve->AddElement(showers);
  }

  top = gEve->GetCurrentEvent();

  gMultiView->DestroyEventRPhi();
  gMultiView->ImportEventRPhi(top);

  gMultiView->DestroyEventRhoZ();
  gMultiView->ImportEventRhoZ(top);
  gEve->Redraw3D(kFALSE, kTRUE);
  
}

void LoadFlashes(float min, float max) {
  if (event.empty()) {
    gMultiView->DestroyEventRPhi();

    gMultiView->DestroyEventRhoZ();
    gEve->Redraw3D(kFALSE, kTRUE);

    return;
  }

  auto top = gEve->GetCurrentEvent();
  std::list < TEveElement *> children;
  top->FindChildren(children, "Op Flashes");
  for (auto
    const & child: children) {
    if (child) {
      child->Destroy();
    }
  }

  if (!FlashTime.empty()) {
  int c = 0;

  for (size_t e = 0; e < FlashTime[spill].size(); e++) {
    if (FlashTime[spill][e] < min || FlashTime[spill][e] > max) {
      continue;
    }
    if (c == 10 || c == 19) {
      c++;
    }

    Float_t x = FlashX[spill][e], y = FlashY[spill][e], z = FlashZ[spill][e];
    Float_t y_width = FlashWidthY[spill][e], z_width = FlashWidthZ[spill][e];

    if (abs(x) == 0) {
      continue;
    }
    if (abs(y_width) <= 2 || abs(y_width) >= 680 || abs(z_width) <= 2 || abs(z_width) >= 680) {
      continue;
    }
    if (isnan(y) || isnan(z)) {
      continue;
    }
    if (isnan(y_width) || isnan(z_width)) {
      continue;
    }
    if (isinf(y) || isinf(z)) {
      continue;
    }
    if (isinf(y_width) || isinf(z_width)) {
      continue;
    }

    auto box = new TEveBox;
    box->SetMainColor(c);
    box->SetMainTransparency(50);

    box->SetVertex(0, x - 1, y - y_width / 2, z - z_width / 2);
    box->SetVertex(1, x - 1, y + y_width / 2, z - z_width / 2);
    box->SetVertex(2, x + 1, y + y_width / 2, z - z_width / 2);
    box->SetVertex(3, x + 1, y - y_width / 2, z - z_width / 2);
    box->SetVertex(4, x - 1, y - y_width / 2, z + z_width / 2);
    box->SetVertex(5, x - 1, y + y_width / 2, z + z_width / 2);
    box->SetVertex(6, x + 1, y + y_width / 2, z + z_width / 2);
    box->SetVertex(7, x + 1, y - y_width / 2, z + z_width / 2);

    box->SetElementName("Op Flashes");
    gEve->AddElement(box);
    }

    auto top = gEve->GetCurrentEvent();

    gMultiView->DestroyEventRPhi();
    gMultiView->ImportEventRPhi(top);

    gMultiView->DestroyEventRhoZ();
    gMultiView->ImportEventRhoZ(top);
    gEve->Redraw3D(kFALSE, kTRUE);
    c++;
  }
}

void LoadCRTHits(float min, float max) {
  if (event.empty()) {
    gMultiView->DestroyEventRPhi();

    gMultiView->DestroyEventRhoZ();
    gEve->Redraw3D(kFALSE, kTRUE);

    return;
  }

  auto top = gEve->GetCurrentEvent();
  auto child = top->FindChild("CRT Hits");
  if (child) {
    child->Destroy();
  }

  if (!CRTX.empty()) {
  auto marker = new TEvePointSet();
  marker->SetOwnIds(kTRUE);
  
  for (size_t e = 0; e < CRTX[spill].size(); e++) {
    if (CRTTime[spill][e] < min || CRTTime[spill][e] > max) {
      continue;
    }

    marker->SetNextPoint(CRTX[spill][e], CRTY[spill][e], CRTZ[spill][e]);
    marker->SetPointId(new TNamed(Form("CRT Point %d", int(e)), ""));
  }
  marker->SetElementName("CRT Hits");
  marker->SetMarkerSize(.4);
  marker->SetMarkerStyle(8);
  marker->SetMainColor(1);
  gEve->AddElement(marker);
  }

  top = gEve->GetCurrentEvent();
  
  gMultiView->DestroyEventRPhi();
  gMultiView->ImportEventRPhi(top);

  gMultiView->DestroyEventRhoZ();
  gMultiView->ImportEventRhoZ(top);
  gEve->Redraw3D(kFALSE, kTRUE);
}

void LoadTPCHits() {
  if (event.empty()) {
    gMultiView->DestroyEventRPhi();

    gMultiView->DestroyEventRhoZ();
    gEve->Redraw3D(kFALSE, kTRUE);

    return;
  }
  if (!SliceID.empty()) {
  std::vector<double>color_ids;
  std::vector<double>unique_indices;
  std::vector<double>::iterator it;

  if (ColorBySlice) {
    color_ids = SliceID[spill];
    unique_indices = SliceID[spill];
    std::sort(unique_indices.begin(), unique_indices.end());
    it = std::unique(unique_indices.begin(), unique_indices.end());
    unique_indices.resize(std::distance(unique_indices.begin(), it));
  } else if (ColorByPFPs) {
    color_ids = PFPID[spill];
    unique_indices = PFPID[spill];
    std::sort(unique_indices.begin(), unique_indices.end());
    it = std::unique(unique_indices.begin(), unique_indices.end());
    unique_indices.resize(std::distance(unique_indices.begin(), it));
  }

  int c = 0;

  for (int ID: unique_indices) {
    if (c == 10 || c == 19) {
      c++;
    }

    auto marker = new TEvePointSet();
    marker->SetOwnIds(kTRUE);

    int target = ID;
    std::vector < int > indices = findItems(color_ids, target);
    int cryo;
    for (auto & e: indices) {

      if (color_ids[e] != ID) {
        continue;
      }

      if (!DrawPlane1 && PlaneID[spill][e] == 0) {
        continue;
      }
      if (!DrawPlane2 && PlaneID[spill][e] == 1) {
        continue;
      }
      if (!DrawPlane3 && PlaneID[spill][e] == 2) {
        continue;
      }

      marker->SetNextPoint(X[spill][e], Y[spill][e], Z[spill][e]);

      if (X[spill][e] > 0) {
        cryo = 1;
      } else {
        cryo = 0;
      }
      marker->SetPointId(new TNamed(Form("Point %d CRYO %d", e, cryo), ""));

    }

    marker->SetMarkerSize(.4);
    marker->SetMarkerStyle(8);
    marker->SetMainColor(1 + c);
    marker->SetElementName("TPC Hits");
    gEve->AddElement(marker);
  }
    auto top = gEve->GetCurrentEvent();

    gMultiView->DestroyEventRPhi();
    gMultiView->ImportEventRPhi(top);

    gMultiView->DestroyEventRhoZ();
    gMultiView->ImportEventRhoZ(top);
    gEve->Redraw3D(kFALSE, kTRUE);
    c++;
  }
}

void doAdvanceSpill(float min, float max) {
  if (spill < nSpills - 1) {
    spill++;
    gEve->GetCurrentEvent()->DestroyElements();
    LoadTPCHits();
    if (PlotCRTHits) {
      LoadCRTHits(min, max);
    }
    if (PlotFlashes) {
      LoadFlashes(min, max);
    }
    if (PlotReco) {
      LoadReco();
    }
    runstring = run[spill];
    eventstring = event[spill];
  }
}
void doPreviousSpill(float min, float max) {
  if (spill > 0) {
    spill--;
    gEve->GetCurrentEvent()->DestroyElements();
    LoadTPCHits();
    if (PlotCRTHits) {
      LoadCRTHits(min, max);
    }
    if (PlotFlashes) {
      LoadFlashes(min, max);
    }
    if (PlotReco) {
      LoadReco();
    }
    runstring = run[spill];
    eventstring = event[spill];
  }
}
void doColorbySlice() {
  ColorByPFPs = false;
  ColorBySlice = true;
  auto top = gEve->GetCurrentEvent();
  std::list < TEveElement *> children;
  top->FindChildren(children, "TPC Hits");
  for (auto
    const & child: children) {
    if (child) {
      child->Destroy();
    }
  }
  LoadTPCHits();
  if (PlotReco) {
    LoadReco();
  }
}
void doColorbyPFP() {
  ColorByPFPs = true;
  ColorBySlice = false;
  auto top = gEve->GetCurrentEvent();
  std::list < TEveElement *> children;
  top->FindChildren(children, "TPC Hits");
  for (auto
    const & child: children) {
    if (child) {
      child->Destroy();
    }
  }
  LoadTPCHits();
  if (PlotReco) {
    LoadReco();
  }
}
void doDrawCRTHits(bool pressed, float min, float max) {
  PlotCRTHits = pressed;
  auto top = gEve->GetCurrentEvent();
  if (PlotCRTHits) {
    LoadCRTHits(min, max);
  } else {
    auto top = gEve->GetCurrentEvent();
    auto child = top->FindChild("CRT Hits");
    if (child) {
      child->Destroy();
    }
  }
}
void doDrawFlashes(bool pressed, float min, float max) {
  PlotFlashes = pressed;
  auto top = gEve->GetCurrentEvent();
  if (PlotFlashes) {
    LoadFlashes(min, max);
  } else {
    auto top = gEve->GetCurrentEvent();
    std::list < TEveElement *> children;
    top->FindChildren(children, "Op Flashes");
    for (auto
      const & child: children) {
      if (child) {
        child->Destroy();
      }
    }
  }
}
void doDrawReco(bool pressed) {
  auto top = gEve->GetCurrentEvent();
  if (pressed) {
    PlotReco = true;
    LoadReco();
  } else {
    PlotReco = false;
    auto top = gEve->GetCurrentEvent();
    std::list < TEveElement *> children;
    top->FindChildren(children, "Reco");
    for (auto
      const & child: children) {
      if (child) {
        child->Destroy();
      }
    }
  }
}
void doUseSliceCuts(std::vector < int > cut_indices) {
  useSliceCuts = true;
  sliceCutIndices = cut_indices;
  GetSpectrumSelection();
  auto top = gEve->GetCurrentEvent();
  std::list < TEveElement *> children;
  top->FindChildren(children, "TPC Hits");
  for (auto
    const & child: children) {
    if (child) {
      child->Destroy();
    }
  }
  LoadTPCHits();
  if (PlotReco) {
    LoadReco();
  }
}
void doUseSpillCuts(std::vector < int > cut_indices) {
  useSpillCuts = true;
  spillCutIndices = cut_indices;
  GetSpectrumSelection();
  auto top = gEve->GetCurrentEvent();
  std::list < TEveElement *> children;
  top->FindChildren(children, "TPC Hits");
  for (auto
    const & child: children) {
    if (child) {
      child->Destroy();
    }
  }
  LoadTPCHits();
  if (PlotReco) {
    LoadReco();
  }
}
void doUseNuSlice(bool pressed) {
  onlyNuSlice = pressed;
  GetSpectrumSelection();
  auto top = gEve->GetCurrentEvent();
  std::list < TEveElement *> children;
  top->FindChildren(children, "TPC Hits");
  for (auto
    const & child: children) {
    if (child) {
      child->Destroy();
    }
  }
  LoadTPCHits();
  if (PlotReco) {
    LoadReco();
  }
}
void doDrawPlane1(bool pressed) {
  DrawPlane1 = pressed;
  auto top = gEve->GetCurrentEvent();
  std::list < TEveElement *> children;
  top->FindChildren(children, "TPC Hits");
  for (auto
    const & child: children) {
    if (child) {
      child->Destroy();
    }
  }
  LoadTPCHits();
}
void doDrawPlane2(bool pressed) {
  DrawPlane2 = pressed;
  auto top = gEve->GetCurrentEvent();
  std::list < TEveElement *> children;
  top->FindChildren(children, "TPC Hits");
  for (auto
    const & child: children) {
    if (child) {
      child->Destroy();
    }
  }
  LoadTPCHits();
}
void doDrawPlane3(bool pressed) {
  DrawPlane3 = pressed;
  auto top = gEve->GetCurrentEvent();
  std::list < TEveElement *> children;
  top->FindChildren(children, "TPC Hits");
  for (auto
    const & child: children) {
    if (child) {
      child->Destroy();
    }
  }
  LoadTPCHits();
}
void doTimeSel(float min, float max) {
  if (PlotCRTHits) {
    LoadCRTHits(min, max);
  }
  if (PlotFlashes) {
    LoadFlashes(min, max);
  }
}

void GetSpectrumSelection() {
  X.clear();
  Y.clear();
  Z.clear();
  PFPID.clear();
  PlaneID.clear();
  SliceID.clear();

  CRTX.clear();
  CRTY.clear();
  CRTZ.clear();
  CRTTime.clear();
  CRTPlane.clear();

  FlashX.clear();
  FlashY.clear();
  FlashZ.clear();
  FlashTime.clear();
  FlashWidthY.clear();
  FlashWidthZ.clear();

  TrackStartX.clear();
  TrackStartY.clear();
  TrackStartZ.clear();
  TrackEndX.clear();
  TrackEndY.clear();
  TrackEndZ.clear();
  TrackPFP.clear();
  TrackSlice.clear();

  ShowerStartX.clear();
  ShowerStartY.clear();
  ShowerStartZ.clear();
  ShowerDirX.clear();
  ShowerDirY.clear();
  ShowerDirZ.clear();
  ShowerLength.clear();
  ShowerAngle.clear();
  ShowerPFP.clear();
  ShowerSlice.clear();

  run.clear();
  event.clear();
  nSpills = 0;
  SpectrumLoader loader(fname);

  const Binning bins = Binning::Simple(1, 0, 1);

  SpillCut thisCut = kNoSpillCut;
  if (useSpillCuts) {
    for (auto cut: spillCutIndices) {
      thisCut = thisCut && spill_cuts[cut];
    }
  }

  Spectrum sFindSpill("", bins, loader, kFindEvents, thisCut, kSpillUnweighted);

  loader.Go();
}

void event_display(const std::string inputName) {
  fname = inputName;
  gSystem->Load("libGeom");
  GetSpectrumSelection();

  TEveManager::Create();

  TEveGeoShape *gentle_geom = 0;

  if (detector == 1) {
    auto file = TFile::Open("guitools/simple_SBND_geom.root");
    if (!file) return;
    auto gse = (TEveGeoShapeExtract *) file->Get("Gentle");
    gentle_geom = TEveGeoShape::ImportShapeExtract(gse, 0);
    file->Close();
  }
  else if (detector == 2) {
    auto file = TFile::Open("guitools/simple_ICARUS_geom.root");
    if (!file) return;
    auto gse = (TEveGeoShapeExtract *) file->Get("Gentle");
    gentle_geom = TEveGeoShape::ImportShapeExtract(gse, 0);
    file->Close();
  }
  else {
    std::cout << "This CAF file is not from a supported detector."  << std::endl;
    std::cout << "Notify Ryan Howell -> rhowell@fnal.gov"  << std::endl;
    exit(EXIT_FAILURE);
  }

  gEve->AddGlobalElement(gentle_geom);

  gMultiView = new MultiView;
  gMultiView->f3DView->GetGLViewer()->SetStyle(TGLRnrCtx::kOutline);

  gMultiView->SetDepth(-10);
  gMultiView->ImportGeomRPhi(gentle_geom);
  gMultiView->ImportGeomRhoZ(gentle_geom);
  gMultiView->SetDepth(0);

  // Final stuff
  //=============

  gEve->GetViewers()->SwitchColorSet();
  gEve->GetDefaultGLViewer()->SetStyle(TGLRnrCtx::kOutline);

  gEve->GetBrowser()->GetTabRight()->SetTab(1);

  new MyMainFrame();

  if (detector == 1) {
    gEve->AddEvent(new TEveEventManager("Event", "SBND CAF Event"));
  }
  else if (detector == 2) {
    gEve->AddEvent(new TEveEventManager("Event", "ICARUS CAF Event"));
  }

  LoadTPCHits();
  LoadReco();
  gEve->Redraw3D(kTRUE); // Reset camera after the first event has been shown.
}
