#include "sbnana/CAFAna/Core/SpectrumLoader.h"
#include "sbnana/CAFAna/Core/Spectrum.h"

#include "sbnana/CAFAna/Core/Binning.h"

#include "guitools/display_vars.h"
#include "tools/data_funcs.h"
#include "guitools/GUI.h"

#include "TFile.h"
#include "TH2.h"
#include "TH3.h"
#include "TAxis3D.h"
#include "TStyle.h"
#include "TPolyMarker3D.h"

using namespace ana;

std::string fname;
std::string title = "3dframe";
TH3F *frame3d = new TH3F(title.c_str(),title.c_str(),1,-400,400,1,-1000,1000,1,-200,200);  

bool ColorByPFPs = false;
bool ColorBySlice = true;

std::vector<double> X;
std::vector<double> Y;
std::vector<double> Z;

std::vector<double> PFPID;
std::vector<double> SliceID;

unsigned int spill = 0; 
unsigned int nSpills = 10;

const SpillVar kFindEvents([](const caf::SRSpillProxy* sr) -> int {
  X = kTPCX(sr);
  Y = kTPCY(sr);
  Z = kTPCZ(sr);
  PFPID = kPFPID(sr);
  SliceID = kSliceID(sr);
  return 42;
});

void Draw()
{
  displayCanvas->Clear();
  frame3d->Draw();
  TAxis3D *axis = TAxis3D::GetPadAxis();
  if (axis) {
    axis->Draw();
    TAxis3D::ToggleZoom();
  }

  if (ColorBySlice) {
    std::vector<double> uniqueSliceIDs = SliceID;
    std::vector<double>::iterator it;
    it = std::unique(uniqueSliceIDs.begin(),uniqueSliceIDs.end());
    uniqueSliceIDs.resize(std::distance(uniqueSliceIDs.begin(),it));

    int c = 0;
    for (int sliceID : uniqueSliceIDs) {
      TPolyMarker3D *plot = new TPolyMarker3D(X.size());
      if (c == 10 || c == 19) { c++; }

      int target = sliceID; 
      std::vector<int> indices = findItems(SliceID, target);
      for (auto &e: indices) {
        if (SliceID[e] != sliceID) { continue; } 
        plot->SetPoint(e,X[e],Z[e],Y[e]);
      }
      plot->SetMarkerSize(.2);
      plot->SetMarkerColor(1+c);
      plot->SetMarkerStyle(20);   
      plot->Draw("SAME");
      c++;
    }
  }
  else if (ColorByPFPs) {
    std::vector<double> uniquePFPs = PFPID;
    std::vector<double>::iterator it;
    std::sort(uniquePFPs.begin(), uniquePFPs.end());
    it = std::unique(uniquePFPs.begin(),uniquePFPs.end());
    uniquePFPs.resize(std::distance(uniquePFPs.begin(),it));
   
    int c = 0;
    for (int pfp : uniquePFPs) {
      TPolyMarker3D *plot = new TPolyMarker3D(X.size());
      if (c == 10 || c == 19) { c++; }

      int target = pfp; 
      std::vector<int> indices = findItems(PFPID, target);
      for (auto &e: indices) {
        if (PFPID[e] != pfp) { continue; } 
        plot->SetPoint(e,X[e],Z[e],Y[e]);
      }
      plot->SetMarkerSize(.2);
      plot->SetMarkerColor(2+c);
      plot->SetMarkerStyle(20);   
      plot->Draw("SAME");
      c++;
    }
  }

  displayCanvas->Modified();
  displayCanvas->Update();
}

void doAdvanceSpill()
{
  if (spill < nSpills - 1) {
    spill++;
    fill_vectors();
    Draw();
  }
}
void doPreviousSpill()
{
  if (spill > 0) {
    spill--;
    fill_vectors();
    Draw();
  }
}
void doColorbySlice()
{
  ColorByPFPs = false;
  ColorBySlice = true;  
  Draw();
}
void doColorbyPFP()
{
  ColorByPFPs = true;
  ColorBySlice = false;  
  Draw();
}
void doUseSliceCuts(bool pressed)
{
  useSliceCuts = pressed;
  fill_vectors();
  Draw();
}
void doUseSpillCuts(bool pressed)
{
  useSpillCuts = pressed;
  fill_vectors();
  Draw();
}
void doUseNuSlice(bool pressed)
{
  onlyNuSlice = pressed;
  fill_vectors();
  Draw();
}

void fill_vectors()
{
  X.clear();
  Y.clear();
  Z.clear();
  PFPID.clear();
  SliceID.clear();
  SpectrumLoader loader(fname);

  const Binning bins = Binning::Simple(100, 0, 5000);

  SpillCut thisCut = kNoSpillCut;
  if (useSpillCuts) {
    for (auto cut : spill_cuts) { thisCut = thisCut && cut; }
  }

  Spectrum sFindSpill("",bins,loader,kFindEvents,thisCut,kSpillUnweighted);
  loader.Select(spill);
}

void event_display(const std::string inputName)
{
  fname = inputName;
  SpectrumLoader loader(fname);
  nSpills = loader.Nevents();

  new MyMainFrame();
  frame3d->SetTitle("Event Display");
  frame3d->GetXaxis()->SetTitle("X position");
  frame3d->GetYaxis()->SetTitle("Z position");
  frame3d->GetZaxis()->SetTitle("Y position");
  frame3d->Draw();
  fill_vectors();
  Draw();
}

