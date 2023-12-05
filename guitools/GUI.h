// Mainframe macro generated from application: /cvmfs/larsoft.opensciencegrid.org/products/root/v6_22_08d/Linux64bit+3.10-2.17-e20-p392-prof/bin/root.exe
// By ROOT version 6.22/08 on 2022-08-27 20:13:17

#ifndef ROOT_TGDockableFrame
#include "TGDockableFrame.h"
#endif
#ifndef ROOT_TGMdiDecorFrame
#include "TGMdiDecorFrame.h"
#endif
#ifndef ROOT_TG3DLine
#include "TG3DLine.h"
#endif
#ifndef ROOT_TGMdiFrame
#include "TGMdiFrame.h"
#endif
#ifndef ROOT_TGMdiMainFrame
#include "TGMdiMainFrame.h"
#endif
#ifndef ROOT_TGMdiMenu
#include "TGMdiMenu.h"
#endif
#ifndef ROOT_TGListBox
#include "TGListBox.h"
#endif
#ifndef ROOT_TGNumberEntry
#include "TGNumberEntry.h"
#endif
#ifndef ROOT_TGScrollBar
#include "TGScrollBar.h"
#endif
#ifndef ROOT_TGComboBox
#include "TGComboBox.h"
#endif
#ifndef ROOT_TGuiBldHintsEditor
#include "TGuiBldHintsEditor.h"
#endif
#ifndef ROOT_TGuiBldNameFrame
#include "TGuiBldNameFrame.h"
#endif
#ifndef ROOT_TGFrame
#include "TGFrame.h"
#endif
#ifndef ROOT_TGMenu
#include "TGMenu.h"
#endif
#ifndef ROOT_TGFileDialog
#include "TGFileDialog.h"
#endif
#ifndef ROOT_TGShutter
#include "TGShutter.h"
#endif
#ifndef ROOT_TGButtonGroup
#include "TGButtonGroup.h"
#endif
#ifndef ROOT_TGCanvas
#include "TGCanvas.h"
#endif
#ifndef ROOT_TGFSContainer
#include "TGFSContainer.h"
#endif
#ifndef ROOT_TGuiBldEditor
#include "TGuiBldEditor.h"
#endif
#ifndef ROOT_TGColorSelect
#include "TGColorSelect.h"
#endif
#ifndef ROOT_TGButton
#include "TGButton.h"
#endif
#ifndef ROOT_TRootContextMenu
#include "TRootContextMenu.h"
#endif
#ifndef ROOT_TGFSComboBox
#include "TGFSComboBox.h"
#endif
#ifndef ROOT_TGLabel
#include "TGLabel.h"
#endif
#ifndef ROOT_TGMsgBox
#include "TGMsgBox.h"
#endif
#ifndef ROOT_TRootGuiBuilder
#include "TRootGuiBuilder.h"
#endif
#ifndef ROOT_TGTab
#include "TGTab.h"
#endif
#ifndef ROOT_TGListView
#include "TGListView.h"
#endif
#ifndef ROOT_TGSplitter
#include "TGSplitter.h"
#endif
#ifndef ROOT_TRootCanvas
#include "TRootCanvas.h"
#endif
#ifndef ROOT_TGStatusBar
#include "TGStatusBar.h"
#endif
#ifndef ROOT_TGListTree
#include "TGListTree.h"
#endif
#ifndef ROOT_TGuiBldGeometryFrame
#include "TGuiBldGeometryFrame.h"
#endif
#ifndef ROOT_TGToolTip
#include "TGToolTip.h"
#endif
#ifndef ROOT_TGToolBar
#include "TGToolBar.h"
#endif
#ifndef ROOT_TRootEmbeddedCanvas
#include "TRootEmbeddedCanvas.h"
#endif
#ifndef ROOT_TCanvas
#include "TCanvas.h"
#endif
#ifndef ROOT_TGuiBldDragManager
#include "TGuiBldDragManager.h"
#endif
#include <TGDoubleSlider.h>
#include "TEvePad.h"
#include "TGLEmbeddedViewer.h"
#include "Riostream.h"
#include <RQ_OBJECT.h>
#include "TColorGradient.h"
#include "TCanvas.h"
#include "TGLViewer.h"
#include "TView3D.h"
#include "button_functions.h"
#include "TSystem.h"
#include "TEveViewer.h"
#include <TEveManager.h>

#include <TEveViewer.h>
#include <TGLViewer.h>

#include <TEveScene.h>

#include <TEveProjectionManager.h>
#include <TEveProjectionAxes.h>

#include <TEveBrowser.h>
#include <TEveWindow.h>
#include "TEveManager.h"
//#include "sbnana/CAFAna/Core/Binning.h"
//#include "sbnana/CAFAna/Core/Cut.h"
//#include "sbnanaobj/StandardRecord/Proxy/SRProxy.h"
#include "GUI_helper.cxx"

class MyMainFrame {
   RQ_OBJECT("MyMainFrame")
private:
   TGMainFrame         *fMain;
   TGVButtonGroup      *fButtonGroup;  // Button group
   TGRadioButton       *fRadiob[2];    // Radio buttons
   TGCheckButton       *fOnlyNuSlice;
   TGCheckButton       *fCRTHits;
   TGCheckButton       *fOpFlashes;
   TGCheckButton       *fReco;
   TGCheckButton       *fCheckPlane1;
   TGCheckButton       *fCheckPlane2;
   TGCheckButton       *fCheckPlane3;
   TGTextButton        *fApplySlcCuts;
   TGTextButton        *fApplySrCuts;
   TGCheckButton       *fRedraw;
   TGStatusBar         *fEventHeader;
   TGListBox           *fSliceBox;
   TGListBox           *fSpillBox;
   TList               *fSelected;
   TGDoubleHSlider     *fTimeSlider;
   TGStatusBar         *fTimeStatus;

public:
   MyMainFrame();
   virtual ~MyMainFrame();
   void DoClose();

   void AdvanceSpill();
   void PreviousSpill();
   void ColorbySlice();
   void ColorbyPFP();
   void DrawCRTHits();
   void DrawFlashes();
   void DrawReco();
   void CheckSliceCut();
   void CheckSpillCut();
   void CheckNuSlice();
   void CheckPlane1();
   void CheckPlane2();
   void CheckPlane3();
   void HandleButtons();
   void DoSlider();
};

MyMainFrame::MyMainFrame() {
   
   TEveBrowser* browser = gEve->GetBrowser();
   browser->StartEmbedding(TRootBrowser::kLeft);
    
   // main frame
   fMain = new TGMainFrame(gClient->GetRoot(),1000,600);
   fMain->SetLayoutBroken(kTRUE);
   fMain->SetWindowName("XX GUI");

   fMain->SetCleanup(kDeepCleanup);
   fMain->Connect("CloseWindow()", "MyMainFrame", this, "DoClose()");
   fMain->DontCallClose(); // to avoid double deletions.

   TGVerticalFrame *controls = new TGVerticalFrame(fMain,50,50);
   TGHorizontalFrame *fCutButtons = new TGHorizontalFrame(controls,50,50);
   TGHorizontalFrame *fCutBoxes = new TGHorizontalFrame(controls,50,50);
   
   Int_t parts[] = {50,50};
   fEventHeader = new TGStatusBar(controls,50,10,kHorizontalFrame);
   fEventHeader->SetParts(parts,2);
   fEventHeader->SetText("Run: ",0);
   fEventHeader->SetText("Event: ",1);
   controls->AddFrame(fEventHeader, new TGLayoutHints(kLHintsTop|kLHintsLeft|
                                                       kLHintsExpandX,5,5,5,10));

   fButtonGroup = new TGVButtonGroup(controls, "Color Options");
   fRadiob[0] = new TGRadioButton(fButtonGroup,new TGHotString("By Slice"));
   fRadiob[0]->Connect("Clicked()","MyMainFrame",this,"ColorbySlice()");
   fRadiob[1] = new TGRadioButton(fButtonGroup,new TGHotString("By PFP"));
   fRadiob[1]->Connect("Clicked()","MyMainFrame",this,"ColorbyPFP()");
   fButtonGroup->SetRadioButtonExclusive(kTRUE);
   fRadiob[0]->SetOn();
   controls->AddFrame(fButtonGroup, new TGLayoutHints(kLHintsTop|kLHintsLeft|
                                                      kLHintsExpandX,5,5,5,10));
   
   fOnlyNuSlice = new TGCheckButton(controls,"Only Plot Nu Slice");
   fOnlyNuSlice->Connect("Clicked()","MyMainFrame",this,"CheckNuSlice()");
   fOnlyNuSlice->SetState(kButtonDown);
   controls->AddFrame(fOnlyNuSlice, new TGLayoutHints(kLHintsTop|kLHintsLeft|
                                                       kLHintsExpandX,5,5,5,10));

   fCRTHits = new TGCheckButton(controls,"Plot CRT Hits");
   fCRTHits->Connect("Clicked()","MyMainFrame",this,"DrawCRTHits()");
   //fCRTHits->SetState(kButtonDown);
   controls->AddFrame(fCRTHits, new TGLayoutHints(kLHintsTop|kLHintsLeft|
                                                       kLHintsExpandX,5,5,5,10));

   fOpFlashes = new TGCheckButton(controls,"Plot Optical Flashes");
   fOpFlashes->Connect("Clicked()","MyMainFrame",this,"DrawFlashes()");
   //fOpFlashes->SetState(kButtonDown);
   controls->AddFrame(fOpFlashes, new TGLayoutHints(kLHintsTop|kLHintsLeft|
                                                       kLHintsExpandX,5,5,5,10));

   fReco = new TGCheckButton(controls,"Plot Reconstructed Objects");
   fReco->Connect("Clicked()","MyMainFrame",this,"DrawReco()");
   fReco->SetState(kButtonDown);
   controls->AddFrame(fReco, new TGLayoutHints(kLHintsTop|kLHintsLeft|
                                                       kLHintsExpandX,5,5,5,10));

   TGVButtonGroup *fhits = new TGVButtonGroup(controls, "Draw Hit Options");
   fCheckPlane1 = new TGCheckButton(fhits,"Induction 1");
   fCheckPlane1->Connect("Clicked()","MyMainFrame",this,"CheckPlane1()");
   fCheckPlane1->SetState(kButtonDown);

   fCheckPlane2 = new TGCheckButton(fhits,"Induction 2");
   fCheckPlane2->Connect("Clicked()","MyMainFrame",this,"CheckPlane2()");
   fCheckPlane2->SetState(kButtonDown);

   fCheckPlane3 = new TGCheckButton(fhits,"Collection");
   fCheckPlane3->Connect("Clicked()","MyMainFrame",this,"CheckPlane3()");
   fCheckPlane3->SetState(kButtonDown);
   controls->AddFrame(fhits, new TGLayoutHints(kLHintsTop|kLHintsLeft|
                                                       kLHintsExpandX,5,5,5,10));

   TGTextButton *NextSpill = new TGTextButton(controls,"Next Spill");
   NextSpill->Connect("Clicked()","MyMainFrame",this,"AdvanceSpill()");
   controls->AddFrame(NextSpill, new TGLayoutHints(kLHintsTop|kLHintsLeft|
                                                       kLHintsExpandX,5,5,5,10));
   TGTextButton *PreviousSpill = new TGTextButton(controls,"Previous Spill");
   PreviousSpill->SetWrapLength(-1);
   PreviousSpill->Connect("Clicked()","MyMainFrame",this,"PreviousSpill()");
   controls->AddFrame(PreviousSpill, new TGLayoutHints(kLHintsTop|kLHintsLeft|
                                                       kLHintsExpandX,5,5,5,10));

   fApplySlcCuts = new TGTextButton(fCutButtons,"Apply Slice Cuts");
   fApplySlcCuts->Resize(105, 80);
   fApplySlcCuts->Connect("Clicked()","MyMainFrame",this,"CheckSliceCut()");
   fCutButtons->AddFrame(fApplySlcCuts, new TGLayoutHints(kLHintsTop|kLHintsLeft,5,5,5,10));

   fApplySrCuts = new TGTextButton(fCutButtons,"Apply Spill Cuts");
   fApplySrCuts->Resize(105, 80);
   fApplySrCuts->Connect("Clicked()","MyMainFrame",this,"CheckSpillCut()");
   fCutButtons->AddFrame(fApplySrCuts, new TGLayoutHints(kLHintsTop|kLHintsRight,20,5,5,10));
   fCutButtons->Resize(230,100);
   controls->AddFrame(fCutButtons);
   
   fSelected = new TList;
   fSliceBox = new TGListBox(fCutBoxes, 90);
   fSliceBox->Resize(100, 80);
   std::vector<std::string> slcCuts = SliceCuts();
   fSliceBox->SetMultipleSelections(kTRUE);

   int i = 0;
   for (auto entry : slcCuts) {
     fSliceBox->AddEntry(entry.c_str(),i);
     i++;
   }
   fSliceBox->MapSubwindows();
   fSliceBox->Layout();
   fCutBoxes->AddFrame(fSliceBox,new TGLayoutHints(kLHintsTop|kLHintsLeft,5,5,5,10));

   fSpillBox = new TGListBox(fCutBoxes, 90);
   fSpillBox->Resize(100, 80);
   std::vector<std::string> srCuts = SpillCuts();
   fSpillBox->SetMultipleSelections(kTRUE);

   i = 0;
   for (auto entry : srCuts) {
     fSpillBox->AddEntry(entry.c_str(), i);
     i++;
   }
   fSpillBox->MapSubwindows();
   fSpillBox->Layout();
   fCutBoxes->AddFrame(fSpillBox,new TGLayoutHints(kLHintsTop|kLHintsRight,15,5,5,10));

   fCutBoxes->Resize(230,100);
   controls->AddFrame(fCutBoxes);

   fTimeSlider = new TGDoubleHSlider(controls,100,kDoubleScaleDownRight,1);
   fTimeSlider->SetRange(-300,300);
   fTimeSlider->SetPosition(0,10);
   fTimeSlider->Connect("PositionChanged()", "MyMainFrame", this, "DoSlider()");
   fTimeSlider->Resize(230,10);
   controls->AddFrame(fTimeSlider);

   Int_t time[] = {100};
   fTimeStatus = new TGStatusBar(controls,50,10,kHorizontalFrame);
   fTimeStatus->SetParts(time,1);
   fTimeStatus->SetText("Time Window: 0.00 us ~ 10.00 us",0);
   fTimeStatus->Resize(230,15);
   controls->AddFrame(fTimeStatus);

   controls->Resize(230,600);

   fMain->AddFrame(controls);

   fMain->MapSubwindows();

   fMain->Resize();
   fMain->MapWindow();
   browser->StopEmbedding();
   browser->SetTabTitle("Event Control", 0);
}

void MyMainFrame::DoClose() {
   // Close window slot: delete this and really close the window
   gSystem->Exec("stty sane");
   delete this;
}

void MyMainFrame::AdvanceSpill() {
  doAdvanceSpill(fTimeSlider->GetMinPosition(),fTimeSlider->GetMaxPosition());
  char s[250] = {0};
  sprintf(s, "Run: %d",runstring);
  fEventHeader->SetText(s,0);
  sprintf(s, "Event: %d",eventstring);
  fEventHeader->SetText(s,1);
}
void MyMainFrame::PreviousSpill() {
  doPreviousSpill(fTimeSlider->GetMinPosition(),fTimeSlider->GetMaxPosition());
  char s[250] = {0};
  sprintf(s, "Run: %d",runstring);
  fEventHeader->SetText(s,0);
  sprintf(s, "Event: %d",eventstring);
  fEventHeader->SetText(s,1);
}
void MyMainFrame::ColorbySlice() {
  doColorbySlice(); 
}
void MyMainFrame::ColorbyPFP() {
  doColorbyPFP(); 
}
void MyMainFrame::CheckSliceCut() {
  fSliceBox->GetSelectedEntries(fSelected);
  TIter next(fSelected);
  TGLBEntry *e;
  std::vector<int> indices;
  while ((e=(TGLBEntry*)next())) {
    indices.push_back(e->EntryId());
  }
  fSelected->Clear();
  doUseSliceCuts(indices,fTimeSlider->GetMinPosition(),fTimeSlider->GetMaxPosition());
}
void MyMainFrame::CheckSpillCut() {
  fSpillBox->GetSelectedEntries(fSelected);
  TIter next(fSelected);
  TGLBEntry *e;
  std::vector<int> indices;
  while ((e=(TGLBEntry*)next())) {
    indices.push_back(e->EntryId());
  }
  fSelected->Clear();
  doUseSpillCuts(indices,fTimeSlider->GetMinPosition(),fTimeSlider->GetMaxPosition());
}
void MyMainFrame::CheckNuSlice() {
  bool pressed = fOnlyNuSlice->GetState() == kButtonDown;
  doUseNuSlice(pressed,fTimeSlider->GetMinPosition(), fTimeSlider->GetMaxPosition());
}
void MyMainFrame::DrawCRTHits() {
  bool pressed = fCRTHits->GetState() == kButtonDown;
  float min = fTimeSlider->GetMinPosition();
  float max  = fTimeSlider->GetMaxPosition();
  doDrawCRTHits(pressed, min, max); 
}
void MyMainFrame::DrawFlashes() {
  bool pressed = fOpFlashes->GetState() == kButtonDown;
  float min = fTimeSlider->GetMinPosition();
  float max  = fTimeSlider->GetMaxPosition();
  doDrawFlashes(pressed, min, max); 
}
void MyMainFrame::DrawReco() {
  bool pressed = fReco->GetState() == kButtonDown;
  doDrawReco(pressed); 
}
void MyMainFrame::CheckPlane1() {
  bool pressed = fCheckPlane1->GetState() == kButtonDown;
  doDrawPlane1(pressed);
}
void MyMainFrame::CheckPlane2() {
  bool pressed = fCheckPlane2->GetState() == kButtonDown;
  doDrawPlane2(pressed);
}
void MyMainFrame::CheckPlane3() {
  bool pressed = fCheckPlane3->GetState() == kButtonDown;
  doDrawPlane3(pressed);
}

void MyMainFrame::DoSlider() {
  doTimeSel(fTimeSlider->GetMinPosition(),fTimeSlider->GetMaxPosition());
  char s[250] = {0};
  sprintf(s, "Time Window: %.2f us ~ %.2f us",fTimeSlider->GetMinPosition(),fTimeSlider->GetMaxPosition());
  fTimeStatus->SetText(s,0);
}

MyMainFrame::~MyMainFrame() {
   delete this;
}
