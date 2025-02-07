#include <TGLayout.h>
#include <TTimer.h>
#include <TSystem.h>

#include "CutHelper.h"
#include "MyMainFrame.h"

//-----------------------------------------------------------------------------
// Constructor: Build the GUI layout
//-----------------------------------------------------------------------------
MyMainFrame::MyMainFrame(const char* fname) 
    : TGMainFrame(gClient->GetRoot(), 1000, 600) {
  // Initialize event display core
  fEventDisplay = new EventDisplay(fname);
  fEventDisplay->InitializeDisplay();

  // Embed in ROOT browser
  TEveBrowser *browser = gEve->GetBrowser();
  browser->StartEmbedding(TRootBrowser::kLeft);

  fSelected = new TList;


  //---------------------------------------------------------------------------
  // Control Panel Layout
  //---------------------------------------------------------------------------
  TGVerticalFrame *controls = new TGVerticalFrame(this, 50, 50);
  
  // Run/Event status display
  Int_t statusParts[] = {50, 50};
  fEventHeader = new TGStatusBar(controls, 50, 10, kHorizontalFrame);
  fEventHeader->SetParts(statusParts, 2);
  controls->AddFrame(fEventHeader, new TGLayoutHints(
    kLHintsTop | kLHintsExpandX, 5, 5, 5, 10));

  // Color scheme selection
  fButtonGroup = new TGVButtonGroup(controls, "Color Coding");
  fRadiob[0] = new TGRadioButton(fButtonGroup, "By Slice");
  fRadiob[1] = new TGRadioButton(fButtonGroup, "By PFP");
  fRadiob[0]->Connect("Clicked()", "MyMainFrame", this, "ColorbySlice()");
  fRadiob[1]->Connect("Clicked()", "MyMainFrame", this, "ColorbyPFP()");
  fButtonGroup->SetRadioButtonExclusive(kTRUE);
  fRadiob[0]->SetOn();
  controls->AddFrame(fButtonGroup, new TGLayoutHints(
    kLHintsTop | kLHintsExpandX, 5, 5, 5, 10));

  //---------------------------------------------------------------------------
  // Display Toggles Section
  //---------------------------------------------------------------------------
  fOnlyNuSlice = new TGCheckButton(controls, "Only Show Neutrino Slice");
  fOnlyNuSlice->Connect("Clicked()", "MyMainFrame", this, "CheckNuSlice()");
  controls->AddFrame(fOnlyNuSlice, new TGLayoutHints(
    kLHintsTop | kLHintsExpandX, 5, 5, 5, 10));

  fCRTHits = new TGCheckButton(controls, "Show CRT Hits");
  fCRTHits->Connect("Clicked()", "MyMainFrame", this, "DrawCRTHits()");
  controls->AddFrame(fCRTHits, new TGLayoutHints(
    kLHintsTop | kLHintsExpandX, 5, 5, 5, 10));

  fOpFlashes = new TGCheckButton(controls, "Show Optical Flashes");
  fOpFlashes->Connect("Clicked()", "MyMainFrame", this, "DrawFlashes()");
  controls->AddFrame(fOpFlashes, new TGLayoutHints(
    kLHintsTop | kLHintsExpandX, 5, 5, 5, 10));

  fReco = new TGCheckButton(controls, "Show Reconstructed Objects");
  fReco->Connect("Clicked()", "MyMainFrame", this, "DrawReco()");
  controls->AddFrame(fReco, new TGLayoutHints(
    kLHintsTop | kLHintsExpandX, 5, 5, 5, 10));

  //---------------------------------------------------------------------------
  // Plane Visibility Controls
  //---------------------------------------------------------------------------
  TGVButtonGroup *planeGroup = new TGVButtonGroup(controls, "Active Planes");
  fCheckPlane[0] = new TGCheckButton(planeGroup, "Induction 1");
  fCheckPlane[1] = new TGCheckButton(planeGroup, "Induction 2");
  fCheckPlane[2] = new TGCheckButton(planeGroup, "Collection");
  for(int i=0; i<3; ++i) {
    fCheckPlane[i]->SetState(kButtonDown);
    fCheckPlane[i]->Connect("Clicked()", "MyMainFrame", this, 
                           Form("CheckPlane%d()", i+1));
  }
  controls->AddFrame(planeGroup, new TGLayoutHints(
    kLHintsTop | kLHintsExpandX, 5, 5, 5, 10));

  //---------------------------------------------------------------------------
  // Navigation Controls
  //---------------------------------------------------------------------------
  TGHorizontalFrame *navFrame = new TGHorizontalFrame(controls);
  TGTextButton *prevBtn = new TGTextButton(navFrame, "Previous Spill");
  TGTextButton *nextBtn = new TGTextButton(navFrame, "Next Spill");
  prevBtn->Connect("Clicked()", "MyMainFrame", this, "PreviousSpill()");
  nextBtn->Connect("Clicked()", "MyMainFrame", this, "AdvanceSpill()");
  navFrame->AddFrame(prevBtn, new TGLayoutHints(kLHintsLeft, 5, 5, 5, 5));
  navFrame->AddFrame(nextBtn, new TGLayoutHints(kLHintsRight, 5, 5, 5, 5));
  controls->AddFrame(navFrame, new TGLayoutHints(
    kLHintsTop | kLHintsExpandX, 5, 5, 5, 10));

  //---------------------------------------------------------------------------
  // Cut Management Section
  //---------------------------------------------------------------------------
  TGHorizontalFrame *cutFrame = new TGHorizontalFrame(controls);
  fSliceBox = new TGListBox(cutFrame, 90);
  fSpillBox = new TGListBox(cutFrame, 90);
  
  // Populate cut lists
  std::vector<std::string> slcCuts = SliceCuts();
  std::vector<std::string> srCuts = SpillCuts();
  for(size_t i=0; i<slcCuts.size(); ++i) 
    fSliceBox->AddEntry(slcCuts[i].c_str(), i);
  for(size_t i=0; i<srCuts.size(); ++i)
    fSpillBox->AddEntry(srCuts[i].c_str(), i);

  cutFrame->AddFrame(fSliceBox, new TGLayoutHints(kLHintsLeft, 5, 5, 5, 5));
  cutFrame->AddFrame(fSpillBox, new TGLayoutHints(kLHintsRight, 5, 5, 5, 5));
  controls->AddFrame(cutFrame, new TGLayoutHints(kLHintsExpandX, 5, 5, 5, 10));

  //---------------------------------------------------------------------------
  // Time Window Controls
  //---------------------------------------------------------------------------
  fTimeSlider = new TGDoubleHSlider(controls, 100, kDoubleScaleDownRight);
  fTimeSlider->SetRange(-300, 300);
  fTimeSlider->Connect("PositionChanged()", "MyMainFrame", this, "DoSlider()");
  controls->AddFrame(fTimeSlider, new TGLayoutHints(
    kLHintsTop | kLHintsExpandX, 5, 5, 5, 5));

  fTimeStatus = new TGStatusBar(controls, 50, 10, kHorizontalFrame);
  fTimeStatus->SetText("Time Window: -300.00 us ~ 300.00 us", 0);
  controls->AddFrame(fTimeStatus, new TGLayoutHints(
    kLHintsTop | kLHintsExpandX, 5, 5, 5, 10));

  // Finalize GUI setup
  AddFrame(controls);
  MapSubwindows();
  Resize();
  MapWindow();
  browser->StopEmbedding("Event Control");

  // Initial display update
  UpdateEventDisplay();
}

//-----------------------------------------------------------------------------
// Destructor
//-----------------------------------------------------------------------------
MyMainFrame::~MyMainFrame() {
  delete fEventDisplay;   
}

//-----------------------------------------------------------------------------
// Event Handlers
//-----------------------------------------------------------------------------

void MyMainFrame::DoClose() {
    Cleanup();
    DeleteWindow();
}

void MyMainFrame::AdvanceSpill() {
  fEventDisplay->NextSpill();
  UpdateEventDisplay();
}

void MyMainFrame::PreviousSpill() {
  fEventDisplay->PreviousSpill();
  UpdateEventDisplay();
}

void MyMainFrame::ColorbySlice() {
  fEventDisplay->SetColorBySlice();
}

void MyMainFrame::ColorbyPFP() {
  fEventDisplay->SetColorByPFP();
}

void MyMainFrame::CheckSliceCut() {
  std::vector<int> cuts;
  fSliceBox->GetSelectedEntries(fSelected);
  TIter next(fSelected);
  while(auto entry = dynamic_cast<TGLBEntry*>(next()))
    cuts.push_back(entry->EntryId());
  fEventDisplay->ApplySliceCuts(cuts);
}

void MyMainFrame::CheckSpillCut() {
  std::vector<int> cuts;
  fSpillBox->GetSelectedEntries(fSelected);
  TIter next(fSelected);
  while(auto entry = dynamic_cast<TGLBEntry*>(next()))
    cuts.push_back(entry->EntryId());
  fEventDisplay->ApplySpillCuts(cuts);
}

void MyMainFrame::CheckNuSlice() {
  bool enable = fOnlyNuSlice->GetState() == kButtonDown;
  fEventDisplay->SetNuSliceFilter(enable);
}

void MyMainFrame::DrawCRTHits() {
  bool show = fCRTHits->GetState() == kButtonDown;
  fEventDisplay->ToggleCRTHits(show);
}

void MyMainFrame::DrawFlashes() {
  bool show = fOpFlashes->GetState() == kButtonDown;
  fEventDisplay->ToggleFlashes(show);
}

void MyMainFrame::DrawReco() {
  bool show = fReco->GetState() == kButtonDown;
  fEventDisplay->ToggleReco(show);
}

void MyMainFrame::CheckPlane1() {
  bool visible = fCheckPlane[0]->GetState() == kButtonDown;
  fEventDisplay->SetPlaneVisibility(0, visible);
}

void MyMainFrame::CheckPlane2() {
  bool visible = fCheckPlane[1]->GetState() == kButtonDown;
  fEventDisplay->SetPlaneVisibility(1, visible);
}

void MyMainFrame::CheckPlane3() {
  bool visible = fCheckPlane[2]->GetState() == kButtonDown;
  fEventDisplay->SetPlaneVisibility(2, visible);
}

//-----------------------------------------------------------------------------
// Time Window Management
//-----------------------------------------------------------------------------
void MyMainFrame::DoSlider() {
  auto min = fTimeSlider->GetMinPosition();
  auto max = fTimeSlider->GetMaxPosition();
  fEventDisplay->SetTimeRange(min, max);
  UpdateEventDisplay();
}

//-----------------------------------------------------------------------------
// Display Status Updates
//-----------------------------------------------------------------------------
void MyMainFrame::UpdateEventDisplay() {
  // Update run/event info
  char status[128];
  snprintf(status, 128, "Run: %d", fEventDisplay->GetCurrentRun());
  fEventHeader->SetText(status, 0);
  snprintf(status, 128, "Event: %d", fEventDisplay->GetCurrentEvent());
  fEventHeader->SetText(status, 1);

  // Update time window display
  auto [tmin, tmax] = fEventDisplay->GetTimeWindow();
  snprintf(status, 128, "Time Window: %.2f us ~ %.2f us", tmin, tmax);
  fTimeStatus->SetText(status, 0);
}
