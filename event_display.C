#define GET_VARIABLE_NAME(Variable) (#Variable)

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
#include "TGeoVolume.h"
#include "TGeoMedium.h"
#include "TEveGeoNode.h"
#include <TEveManager.h>
 
#include <TEveViewer.h>
#include <TGLViewer.h>
 
#include <TEveScene.h>
 
#include <TEveProjectionManager.h>
#include <TEveProjectionAxes.h>
#include <TEveEventManager.h> 
#include <TEveBrowser.h>
#include <TEveWindow.h>

using namespace ana;

const SpillVar kFindEvents([](const caf::SRSpillProxy* sr) -> int {
  slcvars = kSLCVARS(sr);
  srvars = kSRVARS(sr);

  if (slcvars.empty() || srvars.empty()) {
    return 42;
  }

  std::vector<double> xarray;
  std::vector<double> yarray;
  std::vector<double> zarray;
  std::vector<double> pfparray;
  std::vector<double> slicearray;
  std::vector<double> planeidarray;

  std::vector<double> crtxarray;
  std::vector<double> crtyarray;
  std::vector<double> crtzarray;
  std::vector<double> crttimearray;
  std::vector<double> crtplanearray;

  for (size_t i = 0; i<slcvars.size()-kSlcEnd; i+=kSlcEnd) {
    xarray.push_back(slcvars[i+x]);
    yarray.push_back(slcvars[i+y]);
    zarray.push_back(slcvars[i+z]);
    pfparray.push_back(slcvars[i+kpfp]);
    slicearray.push_back(slcvars[i+kslice]);
    planeidarray.push_back(slcvars[i+kplaneid]);
  }

  for (size_t i = 0; i<srvars.size()-kSrEnd; i+=kSrEnd) {
    crtxarray.push_back(srvars[i+crtx]);
    crtyarray.push_back(srvars[i+crty]);
    crtzarray.push_back(srvars[i+crtz]);
    crttimearray.push_back(srvars[i+crttime]);
    crtplanearray.push_back(srvars[i+crtplane]);
  }

  X.push_back(xarray);
  Y.push_back(yarray);
  Z.push_back(zarray);
  PFPID.push_back(pfparray);
  SliceID.push_back(slicearray);
  PlaneID.push_back(planeidarray);

  CRTX.push_back(crtxarray);
  CRTY.push_back(crtyarray);
  CRTZ.push_back(crtzarray);
  CRTTime.push_back(crttimearray);
  CRTPlane.push_back(crtplanearray);

  run.push_back(kRun(sr));
  event.push_back(kEvt(sr));

  nSpills++;

  return 42;
});

void LoadCRTHits(float min, float max)
{
  if (event.empty()) {
      gMultiView->DestroyEventRPhi();

      gMultiView->DestroyEventRhoZ();
      gEve->Redraw3D(kFALSE,kTRUE);

    return;
  }

  auto top = gEve->GetCurrentEvent(); 
  auto child = top->FindChild("CRT Hits");
  if (child) { child->Destroy(); }

  auto marker = new TEvePointSet();
  marker->SetOwnIds(kTRUE);

  for (size_t e = 0; e<CRTX[spill].size(); e++) {
    if (CRTTime[spill][e] < min || CRTTime[spill][e] > max) { continue; }

    marker->SetNextPoint(CRTX[spill][e],CRTY[spill][e],CRTZ[spill][e]);
    marker->SetPointId(new TNamed(Form("CRT Point %d", int(e)), ""));
  }
  marker->SetElementName("CRT Hits");
  marker->SetMarkerSize(.4);
  marker->SetMarkerStyle(8);
  marker->SetMainColor(1);
  gEve->AddElement(marker);
  top = gEve->GetCurrentEvent(); 

  gMultiView->DestroyEventRPhi();
  gMultiView->ImportEventRPhi(top);

  gMultiView->DestroyEventRhoZ();
  gMultiView->ImportEventRhoZ(top);
  gEve->Redraw3D(kFALSE,kTRUE);
}

void LoadTPCHits()
{
  if (event.empty()) {
      gMultiView->DestroyEventRPhi();

      gMultiView->DestroyEventRhoZ();
      gEve->Redraw3D(kFALSE,kTRUE);

    return;
  }


  std::vector<double> color_ids;
  std::vector<double> unique_indices;
  std::vector<double>::iterator it;

  if (ColorBySlice) {
    color_ids = SliceID[spill];
    unique_indices = SliceID[spill];
    std::sort(unique_indices.begin(),unique_indices.end());
    it = std::unique(unique_indices.begin(),unique_indices.end());
    unique_indices.resize(std::distance(unique_indices.begin(),it));
  }
  else if (ColorByPFPs) {
    color_ids = PFPID[spill];
    unique_indices = PFPID[spill];
    std::sort(unique_indices.begin(),unique_indices.end());
    it = std::unique(unique_indices.begin(),unique_indices.end());
    unique_indices.resize(std::distance(unique_indices.begin(),it));
  }

  int c = 0;

  for (int ID : unique_indices) {
    if (c == 10 || c == 19) { c++; }

    auto marker = new TEvePointSet();
    marker->SetOwnIds(kTRUE);

    int target = ID; 
    std::vector<int> indices = findItems(color_ids, target);
    int cryo;
    for (auto &e: indices) {

      if (color_ids[e] != ID) { continue; } 

      if (!DrawPlane1 && PlaneID[spill][e] == 0) { continue; } 
      if (!DrawPlane2 && PlaneID[spill][e] == 1) { continue; } 
      if (!DrawPlane3 && PlaneID[spill][e] == 2) { continue; } 

      marker->SetNextPoint(X[spill][e],Y[spill][e],Z[spill][e]);

      if (X[spill][e] > 0) { cryo=1; }
      else { cryo=0; }
      marker->SetPointId(new TNamed(Form("Point %d CRYO %d", e, cryo), ""));

    }

    marker->SetMarkerSize(.4);
    marker->SetMarkerStyle(8);
    marker->SetMainColor(1+c);
    gEve->AddElement(marker);
    auto top = gEve->GetCurrentEvent();

    gMultiView->DestroyEventRPhi();
    gMultiView->ImportEventRPhi(top);

    gMultiView->DestroyEventRhoZ();
    gMultiView->ImportEventRhoZ(top);
    gEve->Redraw3D(kFALSE,kTRUE);
    c++;
  }
}

void doAdvanceSpill()
{
  if (spill < nSpills - 1) {
    spill++;
    gEve->GetCurrentEvent()->DestroyElements();
    LoadTPCHits();
    runstring = run[spill];
    eventstring = event[spill];
  }
}
void doPreviousSpill()
{
  if (spill > 0) {
    spill--;
    gEve->GetCurrentEvent()->DestroyElements();
    LoadTPCHits();
    runstring = run[spill];
    eventstring = event[spill];
  }
}
void doColorbySlice()
{
  ColorByPFPs = false;
  ColorBySlice = true;  
  gEve->GetCurrentEvent()->DestroyElements();
  LoadTPCHits();
}
void doColorbyPFP()
{
  ColorByPFPs = true;
  ColorBySlice = false;  
  gEve->GetCurrentEvent()->DestroyElements();
  LoadTPCHits();
}
void doDrawCRTHits(bool pressed, float min, float max)
{
  PlotCRTHits = pressed;
  if (PlotCRTHits) { 
    LoadCRTHits(min, max); 
  }
  else {
    auto top = gEve->GetCurrentEvent(); 
    auto child = top->FindChild("CRT Hits");
    if (child) { child->Destroy(); }
  }
}
void doUseSliceCuts(std::vector<int> cut_indices)
{
  useSliceCuts = true;
  sliceCutIndices = cut_indices;
  GetSpectrumSelection();
  gEve->GetCurrentEvent()->DestroyElements();
  LoadTPCHits();
}
void doUseSpillCuts(std::vector<int> cut_indices)
{
  useSpillCuts = true;
  spillCutIndices = cut_indices;
  GetSpectrumSelection();
  gEve->GetCurrentEvent()->DestroyElements();
  LoadTPCHits();
}
void doUseNuSlice(bool pressed)
{
  onlyNuSlice = pressed;
  GetSpectrumSelection();
  gEve->GetCurrentEvent()->DestroyElements();
  LoadTPCHits();
}
void doDrawPlane1(bool pressed)
{
  DrawPlane1 = pressed;
  gEve->GetCurrentEvent()->DestroyElements();
  LoadTPCHits();
}
void doDrawPlane2(bool pressed)
{
  DrawPlane2 = pressed;
  gEve->GetCurrentEvent()->DestroyElements();
  LoadTPCHits();
}
void doDrawPlane3(bool pressed)
{
  DrawPlane3 = pressed;
  gEve->GetCurrentEvent()->DestroyElements();
  LoadTPCHits();
}
void doTimeSel(float min, float max)
{
  if (PlotCRTHits) { LoadCRTHits(min, max); }
}

void GetSpectrumSelection()
{
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

  run.clear();
  event.clear();
  nSpills = 0;
  SpectrumLoader loader(fname);

  const Binning bins = Binning::Simple(1, 0, 1);

  SpillCut thisCut = kNoSpillCut;
  if (useSpillCuts) {
    for (auto cut : spillCutIndices) { 
        thisCut = thisCut && spill_cuts[cut];
    }
  }

  Spectrum sFindSpill("",bins,loader,kFindEvents,thisCut,kSpillUnweighted);

  loader.Go();
}

void event_display(const std::string inputName)
{
   fname = inputName;
   gSystem->Load("libGeom");
   ///// Uncomment if you need to recreate the geometry file
   /*
   TGeoManager *geom = new TGeoManager("geom","ICARUS");

   TGeoMaterial *Vacuum = new TGeoMaterial("vacuum",0,0,0);
   TGeoMaterial *Fe = new TGeoMaterial("Aluminum",55.845,26,7.87);

   TGeoMedium *Air = new TGeoMedium("Air",0,Vacuum);
   TGeoMedium *Iron = new TGeoMedium("Iron",0,Fe);

   TGeoVolume *Phy_Building = geom->MakeBox("top",Air,600,600,2100);
   geom->SetTopVolume(Phy_Building);
   geom->SetTopVisible(0);

   Double_t cryoPosX = 210.215;
   //Double_t cryoPosY = -189.583+164.583;
   Double_t cryoPosZ = 133.483-153.683;
   Double_t cryoPosY = 0;
   //Double_t cryoPosZ = 0;
   Double_t tpcPosX = 74.745;
   //Double_t tpcPosY = -492.475;
   Double_t tpcPosZ = -3.25;
   Double_t tpcPosY = 0;
   //Double_t tpcPosZ = 0;
   Double_t cryo_halfX = 400.43/2;
   Double_t cryo_halfY = 1992.90/2;
   Double_t cryo_halfZ = 426.82/2;
   Double_t tpc_halfX = 149.34/2;
   Double_t tpc_halfY = 984.95;
   Double_t tpc_halfZ = 382.32/2;

   TGeoVolume *CRYO = geom->MakeBox("CRYO", Iron, cryo_halfX, cryo_halfZ, cryo_halfY);;
   CRYO->SetLineColor(18);
   CRYO->SetLineWidth(2);
   CRYO->SetTransparency(90);
   Phy_Building->AddNodeOverlap(CRYO,1,new TGeoTranslation(-cryoPosX, cryoPosZ, cryoPosY));
   Phy_Building->AddNodeOverlap(CRYO,2,new TGeoTranslation( cryoPosX, cryoPosZ, cryoPosY));

   TGeoVolume *TPC = geom->MakeBox("TPC", Iron, tpc_halfX, tpc_halfZ, tpc_halfY);;
   TPC->SetLineColor(18);
   TPC->SetLineWidth(2);
   TPC->SetTransparency(90);
   Phy_Building->AddNodeOverlap(TPC,1,new TGeoTranslation(-cryoPosX-tpcPosX, cryoPosZ-tpcPosZ, cryoPosY-tpcPosY));
   Phy_Building->AddNodeOverlap(TPC,2,new TGeoTranslation(-cryoPosX+tpcPosX, cryoPosZ-tpcPosZ, cryoPosY-tpcPosY));
   Phy_Building->AddNodeOverlap(TPC,3,new TGeoTranslation( cryoPosX-tpcPosX, cryoPosZ-tpcPosZ, cryoPosY-tpcPosY));
   Phy_Building->AddNodeOverlap(TPC,4,new TGeoTranslation( cryoPosX+tpcPosX, cryoPosZ-tpcPosZ, cryoPosY-tpcPosY));

   TEveManager::Create();

   TGeoNode* node = gGeoManager->GetTopNode();
   TEveGeoTopNode* en   = new TEveGeoTopNode(gGeoManager, node);
   en->SetVisLevel(4);
   en->GetNode()->GetVolume()->SetVisibility(kFALSE);
   en->GetNode()->GetVolume()->VisibleDaughters(kTRUE);
   gEve->AddElement(en);
   en->ExpandIntoListTreesRecursively();
   en->SaveExtract("guitools/simple_ICARUS_geom.root","Gentle",kFALSE);
   geom->CloseGeometry();
   gGeoManager->SetNsegments(80);
   
   */

   TEveManager::Create();

   TEveGeoShape *gentle_geom = 0;

   auto file = TFile::Open("guitools/simple_ICARUS_geom.root");
   if (!file) return;
   auto gse = (TEveGeoShapeExtract*) file->Get("Gentle");
   gentle_geom = TEveGeoShape::ImportShapeExtract(gse, 0);
   file->Close();
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

   gEve->AddEvent(new TEveEventManager("Event", "ICARUS CAF Event"));
   GetSpectrumSelection();
   LoadTPCHits();
   gEve->Redraw3D(kTRUE); // Reset camera after the first event has been shown.
}
