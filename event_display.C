#include "sbnana/CAFAna/Core/SpectrumLoader.h"
#include "sbnana/CAFAna/Core/Spectrum.h"

#include "sbnana/CAFAna/Core/Binning.h"

#include "guitools/display_vars.h"
#include "guitools/GUI.h"
#include "guitools/MultiView.h"
#include "tools/data_funcs.h"

#include "TFile.h"
#include "TH2.h"
#include "TH3.h"
#include "TAxis3D.h"
#include "TStyle.h"
#include "TPolyMarker3D.h"
#include "TVirtualViewer3D.h"
#include "TBuffer3D.h"
#include "TBuffer3DTypes.h"
#include "TText.h" 
#include "TObject.h"
#include "TVirtualPad.h"
#include "TAtt3D.h"
#include "TStyle.h" 
#include "TPointSet3D.h"
#include "TPointSet3DGL.h"
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

std::string fname;

bool ColorByPFPs = false;
bool ColorBySlice = true;

std::vector<std::vector<double>> X;
std::vector<std::vector<double>> Y;
std::vector<std::vector<double>> Z;

std::vector<int> run;
std::vector<int> event;

std::vector<std::vector<double>> PFPID;
std::vector<std::vector<double>> SliceID;

MultiView* gMultiView = 0;

int spill = 0; 
int nSpills = 0;

const SpillVar kFindEvents([](const caf::SRSpillProxy* sr) -> int {
  X.push_back(kTPCX(sr));
  Y.push_back(kTPCY(sr));
  Z.push_back(kTPCZ(sr));
  PFPID.push_back(kPFPID(sr));
  SliceID.push_back(kSliceID(sr));
  run.push_back(kRun(sr));
  event.push_back(kEvt(sr));
  nSpills++;
  return 42;
});

void LoadHits()
{
  if (ColorBySlice) {
    std::vector<double> uniqueSliceIDs = SliceID[spill];
    std::vector<double>::iterator it;
    it = std::unique(uniqueSliceIDs.begin(),uniqueSliceIDs.end());
    uniqueSliceIDs.resize(std::distance(uniqueSliceIDs.begin(),it));

    int c = 0;
    auto marker = new TEvePointSet();
    marker->SetOwnIds(kTRUE);
    for (int sliceID : uniqueSliceIDs) {
      if (c == 10 || c == 19) { c++; }

      int target = sliceID; 
      std::vector<int> indices = findItems(SliceID[spill], target);
      char slice[5];
      sprintf(slice,"%d",target);

      for (auto &e: indices) {
        if (SliceID[spill][e] != sliceID) { continue; } 
          marker->SetNextPoint(X[spill][e],Y[spill][e],Z[spill][e]);
          marker->SetPointId(new TNamed(Form("Point %d", e), ""));
      }
      c++;
    }
    marker->SetMarkerSize(.8);
    marker->SetMarkerStyle(2);
    marker->SetMainColor(2);
    gEve->AddElement(marker);
    auto top = gEve->GetCurrentEvent();

    gMultiView->DestroyEventRPhi();
    gMultiView->ImportEventRPhi(top);

    gMultiView->DestroyEventRhoZ();
    gMultiView->ImportEventRhoZ(top);
    gEve->Redraw3D(kFALSE,kTRUE);
  }
  else if (ColorByPFPs) {
    std::vector<double> uniquePFPs = PFPID[spill];
    std::vector<double>::iterator it;
    std::sort(uniquePFPs.begin(), uniquePFPs.end());
    it = std::unique(uniquePFPs.begin(),uniquePFPs.end());
    uniquePFPs.resize(std::distance(uniquePFPs.begin(),it));
   
    int c = 0;
    auto marker = new TEvePointSet();
    marker->SetOwnIds(kTRUE);

    for (int pfp : uniquePFPs) {
      if (c == 10 || c == 19) { c++; }

      int target = pfp; 
      std::vector<int> indices = findItems(PFPID[spill], target);
      char slice[252];
      sprintf(slice,"%d",target);
      auto marker = new TEvePointSet(slice,indices.size());
      marker->SetOwnIds(kTRUE);
      marker->SetElementName(slice);
      marker->SetMarkerSize(.5);
      marker->SetMarkerStyle(2);

      for (auto &e: indices) {
        if (PFPID[spill][e] != pfp) { continue; } 
          marker->SetNextPoint(X[spill][e],Y[spill][e],Z[spill][e]);
          marker->SetPointId(new TNamed(Form("Point %d", e), ""));
      }
      c++;
    }
    marker->SetMarkerSize(.8);
    marker->SetMarkerStyle(2);
    marker->SetMainColor(2);
    gEve->AddElement(marker);
    auto top = gEve->GetCurrentEvent();

    gMultiView->DestroyEventRPhi();
    gMultiView->ImportEventRPhi(top);

    gMultiView->DestroyEventRhoZ();
    gMultiView->ImportEventRhoZ(top);
    gEve->Redraw3D(kFALSE,kTRUE);
  }
  
}

void doAdvanceSpill()
{
  if (spill < nSpills - 1) {
    spill++;
    gEve->GetCurrentEvent()->DestroyElements();
    LoadHits();
    runstring = run[spill];
    eventstring = event[spill];
  }
}
void doPreviousSpill()
{
  if (spill > 0) {
    spill--;
    gEve->GetCurrentEvent()->DestroyElements();
    LoadHits();
    runstring = run[spill];
    eventstring = event[spill];
  }
}
void doColorbySlice()
{
  ColorByPFPs = false;
  ColorBySlice = true;  
  LoadHits();
}
void doColorbyPFP()
{
  ColorByPFPs = true;
  ColorBySlice = false;  
  LoadHits();
}
void doUseSliceCuts(bool pressed)
{
  useSliceCuts = pressed;
  GetSpectrumSelection();
  LoadHits();
}
void doUseSpillCuts(bool pressed)
{
  useSpillCuts = pressed;
  GetSpectrumSelection();
  LoadHits();
}
void doUseNuSlice(bool pressed)
{
  onlyNuSlice = pressed;
  GetSpectrumSelection();
  LoadHits();
}

void GetSpectrumSelection()
{
  X.clear();
  Y.clear();
  Z.clear();
  PFPID.clear();
  SliceID.clear();
  run.clear();
  event.clear();
  nSpills = 0;
  SpectrumLoader loader(fname);

  const Binning bins = Binning::Simple(100, 0, 5000);

  SpillCut thisCut = kNoSpillCut;
  if (useSpillCuts) {
    for (auto cut : spill_cuts) { thisCut = thisCut && cut; }
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
   Phy_Building->AddNodeOverlap(CRYO,1,new TGeoTranslation(-220.215, 0, -155.1+134.9));
   Phy_Building->AddNodeOverlap(CRYO,2,new TGeoTranslation( 220.215, 0, -155.1+134.9));

   TGeoVolume *TPC = geom->MakeBox("TPC", Iron, tpc_halfX, tpc_halfZ, tpc_halfY);;
   TPC->SetLineColor(18);
   TPC->SetLineWidth(2);
   TPC->SetTransparency(90);
   Phy_Building->AddNodeOverlap(TPC,1,new TGeoTranslation(-220.215-74.745, 0, -155.1+134.9+3.25));
   Phy_Building->AddNodeOverlap(TPC,2,new TGeoTranslation(-220.215+74.745, 0, -155.1+134.9+3.25));
   Phy_Building->AddNodeOverlap(TPC,3,new TGeoTranslation( 220.215-74.745, 0, -155.1+134.9+3.25));
   Phy_Building->AddNodeOverlap(TPC,4,new TGeoTranslation( 220.215+74.745, 0, -155.1+134.9+3.25));

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
   //gentle_geom->SetMainTransparency(100);
   gEve->AddGlobalElement(gentle_geom);

   gMultiView = new MultiView;
//   gMultiView->f3DView->GetGLViewer()->SetStyle(TGLRnrCtx::kOutline);

   gMultiView->SetDepth(-10);
   gMultiView->ImportGeomRPhi(gentle_geom);
   gMultiView->ImportGeomRhoZ(gentle_geom);
   gMultiView->SetDepth(0);

   // Final stuff
   //=============

   gEve->GetViewers()->SwitchColorSet();
   gEve->GetDefaultGLViewer()->SetStyle(TGLRnrCtx::kOutline);

   gEve->GetBrowser()->GetTabRight()->SetTab(1);

   MyMainFrame* main = new MyMainFrame();

   gEve->AddEvent(new TEveEventManager("Event", "ICARUS CAF Event"));
   GetSpectrumSelection();
   LoadHits();
   gEve->Redraw3D(kTRUE); // Reset camera after the first event has been shown.
}
