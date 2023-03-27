#include "sbnana/CAFAna/Core/SpectrumLoader.h"
#include "sbnana/CAFAna/Core/Spectrum.h"

#include "sbnana/CAFAna/Core/Binning.h"

#include "guitools/display_vars.h"
#include "guitools/GUI.h"
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
#include "TGLRnrCtx.h"

using namespace ana;

std::string fname;
std::string title = "3dframe";
TH3F *frame3d = new TH3F(title.c_str(),title.c_str(),1,-420.43,420.43,1,-996.1,996.1,1,-233.61,193.21);

bool ColorByPFPs = false;
bool ColorBySlice = true;

std::vector<std::vector<double>> X;
std::vector<std::vector<double>> Y;
std::vector<std::vector<double>> Z;

std::vector<int> run;
std::vector<int> event;

std::vector<std::vector<double>> PFPID;
std::vector<std::vector<double>> SliceID;

int spill = 0; 
int nSpills = 0;

class Shape : public TObject
{
public:
   Shape(Int_t color, Double_t x, Double_t y, Double_t z);
   ~Shape() {};
   virtual TBuffer3D & GetBuffer3D(UInt_t reqSections) = 0;

protected:
   Double_t fX, fY, fZ;    // Origin
   Int_t fColor;

   ClassDef(Shape,0);
};

ClassImp(Shape);

Shape::Shape(Int_t color, Double_t x, Double_t y, Double_t z) :
   fX(x), fY(y), fZ(z), fColor(color)
{}

class Box : public Shape
{
public:
   Box(Int_t color, Double_t x, Double_t y, Double_t z,
       Double_t dX, Double_t dY, Double_t dZ);
   ~Box() {};

   virtual TBuffer3D & GetBuffer3D(UInt_t reqSections);

private:
   Double_t fDX, fDY, fDZ; // Half lengths

   ClassDef(Box,0);
};

ClassImp(Box);

Box::Box(Int_t color, Double_t x, Double_t y, Double_t z,
         Double_t dX, Double_t dY, Double_t dZ) :
   Shape(color,x,y,z),
   fDX(dX), fDY(dY), fDZ(dZ)
{}

TBuffer3D & Box::GetBuffer3D(UInt_t reqSections)
{
   static TBuffer3D buffer(TBuffer3DTypes::kGeneric);

   // Complete kCore section - this could be moved to Shape base class
   if (reqSections & TBuffer3D::kCore) {
      buffer.ClearSectionsValid();
      buffer.fID = this;
      buffer.fColor = fColor;       // Color index - see gROOT->GetColor()
      buffer.fTransparency = 50;     // Transparency 0 (opaque) - 100 (fully transparent)
      buffer.fLocalFrame = kFALSE;
      buffer.SetLocalMasterIdentity();
      buffer.fReflection = kFALSE;
      buffer.SetSectionsValid(TBuffer3D::kCore);
   }
   // Complete kBoundingBox section
   if (reqSections & TBuffer3D::kBoundingBox) {
      Double_t origin[3] = { fX, fY, fZ };
      Double_t halfLength[3] =  { fDX, fDY, fDZ };
      buffer.SetAABoundingBox(origin, halfLength);
      buffer.SetSectionsValid(TBuffer3D::kBoundingBox);
   }
   // No kShapeSpecific section

   // Complete kRawSizes section
   if (reqSections & TBuffer3D::kRawSizes) {
      buffer.SetRawSizes(8, 3*8, 12, 3*12, 6, 6*6);
      buffer.SetSectionsValid(TBuffer3D::kRawSizes);
   }
   // Complete kRaw section
   if (reqSections & TBuffer3D::kRaw) {
      // Points (8)
      // 3 components: x,y,z
      buffer.fPnts[ 0] = fX - fDX; buffer.fPnts[ 1] = fY - fDY; buffer.fPnts[ 2] = fZ - fDZ; // 0
      buffer.fPnts[ 3] = fX + fDX; buffer.fPnts[ 4] = fY - fDY; buffer.fPnts[ 5] = fZ - fDZ; // 1
      buffer.fPnts[ 6] = fX + fDX; buffer.fPnts[ 7] = fY + fDY; buffer.fPnts[ 8] = fZ - fDZ; // 2
      buffer.fPnts[ 9] = fX - fDX; buffer.fPnts[10] = fY + fDY; buffer.fPnts[11] = fZ - fDZ; // 3
      buffer.fPnts[12] = fX - fDX; buffer.fPnts[13] = fY - fDY; buffer.fPnts[14] = fZ + fDZ; // 4
      buffer.fPnts[15] = fX + fDX; buffer.fPnts[16] = fY - fDY; buffer.fPnts[17] = fZ + fDZ; // 5
      buffer.fPnts[18] = fX + fDX; buffer.fPnts[19] = fY + fDY; buffer.fPnts[20] = fZ + fDZ; // 6
      buffer.fPnts[21] = fX - fDX; buffer.fPnts[22] = fY + fDY; buffer.fPnts[23] = fZ + fDZ; // 7

      // Segments (12)
      // 3 components: segment color(ignored), start point index, end point index
      // Indexes reference the above points
      buffer.fSegs[ 0] = fColor   ; buffer.fSegs[ 1] = 0   ; buffer.fSegs[ 2] = 1   ; // 0
      buffer.fSegs[ 3] = fColor   ; buffer.fSegs[ 4] = 1   ; buffer.fSegs[ 5] = 2   ; // 1
      buffer.fSegs[ 6] = fColor   ; buffer.fSegs[ 7] = 2   ; buffer.fSegs[ 8] = 3   ; // 2
      buffer.fSegs[ 9] = fColor   ; buffer.fSegs[10] = 3   ; buffer.fSegs[11] = 0   ; // 3
      buffer.fSegs[12] = fColor   ; buffer.fSegs[13] = 4   ; buffer.fSegs[14] = 5   ; // 4
      buffer.fSegs[15] = fColor   ; buffer.fSegs[16] = 5   ; buffer.fSegs[17] = 6   ; // 5
      buffer.fSegs[18] = fColor   ; buffer.fSegs[19] = 6   ; buffer.fSegs[20] = 7   ; // 6
      buffer.fSegs[21] = fColor   ; buffer.fSegs[22] = 7   ; buffer.fSegs[23] = 4   ; // 7
      buffer.fSegs[24] = fColor   ; buffer.fSegs[25] = 0   ; buffer.fSegs[26] = 4   ; // 8
      buffer.fSegs[27] = fColor   ; buffer.fSegs[28] = 1   ; buffer.fSegs[29] = 5   ; // 9
      buffer.fSegs[30] = fColor   ; buffer.fSegs[31] = 2   ; buffer.fSegs[32] = 6   ; // 10
      buffer.fSegs[33] = fColor   ; buffer.fSegs[34] = 3   ; buffer.fSegs[35] = 7   ; // 11

      // Polygons (6)
      // 5+ (2+n) components: polygon color (ignored), segment count(n=3+),
      // seg1, seg2 .... segn index
      // Segments indexes refer to the above 12 segments
      // Here n=4 - each polygon defines a rectangle - 4 sides.
      buffer.fPols[ 0] = fColor   ; buffer.fPols[ 1] = 4   ;  buffer.fPols[ 2] = 8  ; // 0
      buffer.fPols[ 3] = 4        ; buffer.fPols[ 4] = 9   ;  buffer.fPols[ 5] = 0  ;
      buffer.fPols[ 6] = fColor   ; buffer.fPols[ 7] = 4   ;  buffer.fPols[ 8] = 9  ; // 1
      buffer.fPols[ 9] = 5        ; buffer.fPols[10] = 10  ;  buffer.fPols[11] = 1  ;
      buffer.fPols[12] = fColor   ; buffer.fPols[13] = 4   ;  buffer.fPols[14] = 10  ; // 2
      buffer.fPols[15] = 6        ; buffer.fPols[16] = 11  ;  buffer.fPols[17] = 2  ;
      buffer.fPols[18] = fColor   ; buffer.fPols[19] = 4   ;  buffer.fPols[20] = 11 ; // 3
      buffer.fPols[21] = 7        ; buffer.fPols[22] = 8   ;  buffer.fPols[23] = 3 ;
      buffer.fPols[24] = fColor   ; buffer.fPols[25] = 4   ;  buffer.fPols[26] = 1  ; // 4
      buffer.fPols[27] = 2        ; buffer.fPols[28] = 3   ;  buffer.fPols[29] = 0  ;
      buffer.fPols[30] = fColor   ; buffer.fPols[31] = 4   ;  buffer.fPols[32] = 7  ; // 5
      buffer.fPols[33] = 6        ; buffer.fPols[34] = 5   ;  buffer.fPols[35] = 4  ;

      buffer.SetSectionsValid(TBuffer3D::kRaw);
  }

   return buffer;
}
class MyGeom : public TObject, public TAtt3D
{
public:
   MyGeom();
   ~MyGeom();
 
   void Draw(Option_t *option);
   void Paint(Option_t *option);
 
private:
   std::vector<Shape *> fShapes;
 
   ClassDef(MyGeom,0);
};
 
ClassImp(MyGeom);
 
MyGeom::MyGeom()
{
   Shape * CRYOE;
   Shape * CRYOW;

   Shape * TPCEE;
   Shape * TPCEW;
   Shape * TPCWE;
   Shape * TPCWW;

   TPCEE = new Box(kRed, -220.215-74.745, 0, -155.1+134.9+3.25, 149.34/2, 984.95, 382.32/2);
   TPCEW = new Box(kRed, -220.215+74.745, 0, -155.1+134.9+3.25, 149.34/2, 984.95, 382.32/2);
   TPCWE = new Box(kRed,  220.215-74.745, 0, -155.1+134.9+3.25, 149.34/2, 984.95, 382.32/2);
   TPCWW = new Box(kRed,  220.215+74.745, 0, -155.1+134.9+3.25, 149.34/2, 984.95, 382.32/2);
   
   CRYOE = new Box(kBlack, -220.215, 0, -155.1+134.9, 400.43/2, 1992.90/2, 426.82/2);
   CRYOW = new Box(kBlack,  220.215, 0, -155.1+134.9, 400.43/2, 1992.90/2, 426.82/2);

   //fShapes.push_back(CRYOE);
   //fShapes.push_back(CRYOW);
   fShapes.push_back(TPCEE);
   fShapes.push_back(TPCEW);
   fShapes.push_back(TPCWE);
   fShapes.push_back(TPCWW);
}
 
MyGeom::~MyGeom()
{
   // Clear out fShapes
}
void MyGeom::Draw(Option_t *option)
{
   TObject::Draw(option);
 
   // Ask pad to create 3D viewer of type 'option'
   gPad->GetViewer3D(option);
}
 
void MyGeom::Paint(Option_t * /*option*/)
{
   TVirtualViewer3D * viewer = gPad->GetViewer3D();
 
   // If MyGeom derives from TAtt3D then pad will recognise
   // that the object it is asking to paint is 3D, and open/close
   // the scene for us. If not Open/Close are required
   //viewer->BeginScene();
 
   // We are working in the master frame - so we don't bother
   // to ask the viewer if it prefers local. Viewer's must
   // always support master frame as minimum. c.f. with
   // viewer3DLocal.C
   std::vector<Shape *>::const_iterator ShapeIt = fShapes.begin();
   Shape * shape;
   while (ShapeIt != fShapes.end()) {
      shape = *ShapeIt;
 
      UInt_t reqSections = TBuffer3D::kCore|TBuffer3D::kBoundingBox|TBuffer3D::kShapeSpecific;
      TBuffer3D & buffer = shape->GetBuffer3D(reqSections);
      reqSections = viewer->AddObject(buffer);
 
      if (reqSections != TBuffer3D::kNone) {
         shape->GetBuffer3D(reqSections);
         viewer->AddObject(buffer);
      }
      ShapeIt++;
   }
   // Not required as we are TAtt3D subclass
   //viewer->EndScene();
}

MyGeom * myGeom = new MyGeom;

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

void Draw()
{
  displayCanvas->Clear();
  gStyle->SetCanvasPreferGL(true);
  //frame3d->Draw();
  myGeom->Draw("OGL");

  if (ColorBySlice) {
    std::vector<double> uniqueSliceIDs = SliceID[spill];
    std::vector<double>::iterator it;
    it = std::unique(uniqueSliceIDs.begin(),uniqueSliceIDs.end());
    uniqueSliceIDs.resize(std::distance(uniqueSliceIDs.begin(),it));

    int c = 0;
    for (int sliceID : uniqueSliceIDs) {
      TPolyMarker3D *plot = new TPolyMarker3D(X[spill].size());
      TPointSet3D *marker = new TPointSet3D(X[spill].size());
      if (c == 10 || c == 19) { c++; }

      int target = sliceID; 
      std::vector<int> indices = findItems(SliceID[spill], target);
      for (auto &e: indices) {
        if (SliceID[spill][e] != sliceID) { continue; } 
         marker->SetPoint(e,X[spill][e],Z[spill][e],Y[spill][e]);
         marker->SetMarkerColor(1+c);
      }
      c++;
      TPointSet3DGL *glrender = (TPointSet3DGL*)marker;
      TGLViewer *viewer = (TGLViewer*)gPad->GetViewer3D();
      glrender->DirectDraw(*viewer->GetRnrCtx());
      //marker->Draw("OGL");
    }
  }
  else if (ColorByPFPs) {
    std::vector<double> uniquePFPs = PFPID[spill];
    std::vector<double>::iterator it;
    std::sort(uniquePFPs.begin(), uniquePFPs.end());
    it = std::unique(uniquePFPs.begin(),uniquePFPs.end());
    uniquePFPs.resize(std::distance(uniquePFPs.begin(),it));
   
    int c = 0;
    for (int pfp : uniquePFPs) {
      TPolyMarker3D *plot = new TPolyMarker3D(X[spill].size());
      if (c == 10 || c == 19) { c++; }

      int target = pfp; 
      std::vector<int> indices = findItems(PFPID[spill], target);
      for (auto &e: indices) {
        if (PFPID[spill][e] != pfp) { continue; } 
        plot->SetPoint(e,X[spill][e],Z[spill][e],Y[spill][e]);
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
    Draw();
    runstring = run[spill];
    eventstring = event[spill];
  }
}
void doPreviousSpill()
{
  if (spill > 0) {
    spill--;
    Draw();
    runstring = run[spill];
    eventstring = event[spill];
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
  gStyle->SetCanvasPreferGL(true);
  gStyle->SetOptStat(0);
  fname = inputName;
  new MyMainFrame();
  frame3d->SetTitle("Event Display");
  frame3d->GetXaxis()->SetTitle("X position");
  frame3d->GetYaxis()->SetTitle("Z position");
  frame3d->GetZaxis()->SetTitle("Y position");
  //frame3d->Draw();
  fill_vectors();
  myGeom->Draw("OGL");
  Draw();
}
