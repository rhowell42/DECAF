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

#include "TEvePad.h"
#include "TGLEmbeddedViewer.h"
#include "Riostream.h"
#include <RQ_OBJECT.h>
#include "TColorGradient.h"
#include "TCanvas.h"
#include "TGLViewer.h"
#include "TView3D.h"
#include "button_functions.h"

TCanvas *displayCanvas;

class MyMainFrame {
   RQ_OBJECT("MyMainFrame")
private:
   TGMainFrame         *fMain;
   TRootEmbeddedCanvas *fRootEmbeddedCanvas;
   TGVButtonGroup      *fButtonGroup;  // Button group
   TGRadioButton       *fRadiob[2];    // Radio buttons
   TGCheckButton       *fOnlyNuSlice;
   TGCheckButton       *fApplySlcCuts;
   TGCheckButton       *fApplySrCuts;

public:
   MyMainFrame();
   virtual ~MyMainFrame();
   void DoClose();

   void AdvanceSpill();
   void PreviousSpill();
   void ColorbySlice();
   void ColorbyPFP();
   void CheckSliceCut();
   void CheckSpillCut();
   void CheckNuSlice();
};

MyMainFrame::MyMainFrame() {
   // main frame
   
   
   fMain = new TGMainFrame(gClient->GetRoot(),10,10,kMainFrame | kVerticalFrame);
   fMain->SetName("fMain");
   fMain->SetLayoutBroken(kTRUE);

   fMain->SetCleanup(kDeepCleanup);
   fMain->Connect("CloseWindow()", "MyMainFrame", this, "DoClose()");
   fMain->DontCallClose(); // to avoid double deletions.

   fButtonGroup = new TGVButtonGroup(fMain, "Color Options");
   fRadiob[0] = new TGRadioButton(fButtonGroup,new TGHotString("By Slice"));
   fRadiob[0]->Connect("Clicked()","MyMainFrame",this,"ColorbySlice()");
   fRadiob[1] = new TGRadioButton(fButtonGroup,new TGHotString("By PFP"));
   fRadiob[1]->Connect("Clicked()","MyMainFrame",this,"ColorbyPFP()");
   fMain->AddFrame(fButtonGroup,new TGLayoutHints(kLHintsCenterX | kLHintsCenterY,1, 1, 1, 1));
   fButtonGroup->SetRadioButtonExclusive(kTRUE);
   fRadiob[0]->SetOn();
   fButtonGroup->MoveResize(120,380,104,60);
   
   fOnlyNuSlice = new TGCheckButton(fMain,"Only Plot Nu Slice");
   fOnlyNuSlice->SetTextJustify(36);
   fOnlyNuSlice->SetMargins(0,0,0,0);
   fOnlyNuSlice->SetWrapLength(-1);
   fOnlyNuSlice->Connect("Clicked()","MyMainFrame",this,"CheckNuSlice()");
   fOnlyNuSlice->SetState(kButtonDown);
   fMain->AddFrame(fOnlyNuSlice, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   fOnlyNuSlice->MoveResize(240,400,140,17);

   fApplySlcCuts = new TGCheckButton(fMain,"Apply Slice Cuts");
   fApplySlcCuts->SetTextJustify(36);
   fApplySlcCuts->SetMargins(0,0,0,0);
   fApplySlcCuts->SetWrapLength(-1);
   fApplySlcCuts->Connect("Clicked()","MyMainFrame",this,"CheckSliceCut()");
   fMain->AddFrame(fApplySlcCuts, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   fApplySlcCuts->MoveResize(390,385,140,17);

   fApplySrCuts = new TGCheckButton(fMain,"Apply Spill Cuts");
   fApplySrCuts->SetTextJustify(36);
   fApplySrCuts->SetMargins(0,0,0,0);
   fApplySrCuts->SetWrapLength(-1);
   fApplySrCuts->Connect("Clicked()","MyMainFrame",this,"CheckSpillCut()");
   fMain->AddFrame(fApplySrCuts, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   fApplySrCuts->MoveResize(390,410,140,17);

   TGTextButton *NextSpill = new TGTextButton(fMain,"Next Spill",-1,TGTextButton::GetDefaultGC()(),TGTextButton::GetDefaultFontStruct(),kRaisedFrame);
   NextSpill->SetTextJustify(36);
   NextSpill->SetMargins(0,0,0,0);
   NextSpill->SetWrapLength(-1);
   NextSpill->Resize(88,16);
   NextSpill->Connect("Clicked()","MyMainFrame",this,"AdvanceSpill()");
   fMain->AddFrame(NextSpill, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   NextSpill->MoveResize(16,385,88,16);

   TGTextButton *PreviousSpill = new TGTextButton(fMain,"Previous Spill",-1,TGTextButton::GetDefaultGC()(),TGTextButton::GetDefaultFontStruct(),kRaisedFrame);
   PreviousSpill->SetTextJustify(36);
   PreviousSpill->SetMargins(0,0,0,0);
   PreviousSpill->SetWrapLength(-1);
   PreviousSpill->Resize(88,16);
   PreviousSpill->Connect("Clicked()","MyMainFrame",this,"PreviousSpill()");
   fMain->AddFrame(PreviousSpill, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   PreviousSpill->MoveResize(16,410,88,16);

   // embedded canvas
   fRootEmbeddedCanvas = new TRootEmbeddedCanvas(0,fMain,464,280,kSunkenFrame);
   fRootEmbeddedCanvas->SetName("fRootEmbeddedCanvas");
   Int_t wfRootEmbeddedCanvas = fRootEmbeddedCanvas->GetCanvasWindowId();

   displayCanvas = new TCanvas("displayCanvas", 10, 10, wfRootEmbeddedCanvas);

   fRootEmbeddedCanvas->AdoptCanvas(displayCanvas);
   fMain->AddFrame(fRootEmbeddedCanvas, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   fRootEmbeddedCanvas->MoveResize(8,8,688,352);

   fMain->SetMWMHints(kMWMDecorAll,
                        kMWMFuncAll,
                        kMWMInputModeless);
   fMain->MapSubwindows();

   fMain->Resize(fMain->GetDefaultSize());
   fMain->MapWindow();
   fMain->Resize(711,437);
}

void MyMainFrame::DoClose() {
   // Close window slot: delete this and really close the window
   delete this;
}

void MyMainFrame::AdvanceSpill() {
  doAdvanceSpill(); 
}
void MyMainFrame::PreviousSpill() {
  doPreviousSpill(); 
}
void MyMainFrame::ColorbySlice() {
  doColorbySlice(); 
}
void MyMainFrame::ColorbyPFP() {
  doColorbyPFP(); 
}
void MyMainFrame::CheckSliceCut() {
  bool pressed = fApplySlcCuts->GetState() == kButtonDown;
  doUseSliceCuts(pressed);
}
void MyMainFrame::CheckSpillCut() {
  bool pressed = fApplySrCuts->GetState() == kButtonDown;
  doUseSpillCuts(pressed);
}
void MyMainFrame::CheckNuSlice() {
  bool pressed = fOnlyNuSlice->GetState() == kButtonDown;
  doUseNuSlice(pressed);
}

MyMainFrame::~MyMainFrame() {
   delete this;
}
