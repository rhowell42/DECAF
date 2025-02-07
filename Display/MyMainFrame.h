#ifndef MY_MAIN_FRAME_H
#define MY_MAIN_FRAME_H

// ROOT GUI Headers
#include <TGFrame.h>
#include <TGButton.h>
#include <TGListBox.h>
#include <TGSlider.h>
#include <TGButtonGroup.h>
#include <TGDoubleSlider.h>
#include <TGStatusBar.h>
#include <TEveBrowser.h>
#include <RQ_OBJECT.h>
#include "TGMdiMainFrame.h"

// Event Display Components
#include "EventDisplay.h"

class MyMainFrame : public TGMainFrame {
  RQ_OBJECT("MyMainFrame")
private:
  // GUI Components
  TGVButtonGroup *fButtonGroup;     // Color scheme selection
  TGRadioButton *fRadiob[2];        // [0] Slice color, [1] PFP color
  TGCheckButton *fOnlyNuSlice;      // NuSlice filter toggle
  TGCheckButton *fCRTHits;          // CRT hits display toggle
  TGCheckButton *fOpFlashes;        // Optical flashes toggle
  TGCheckButton *fReco;             // Reconstructed objects toggle
  TGCheckButton *fCheckPlane[3];    // Plane visibility toggles [0-2]
  TGTextButton *fApplySlcCuts;      // Apply slice cuts button
  TGTextButton *fApplySrCuts;       // Apply spill cuts button
  TGStatusBar *fEventHeader;        // Run/event display
  TGListBox *fSliceBox;             // Available slice cuts
  TGListBox *fSpillBox;             // Available spill cuts
  TList *fSelected;                 // Temporary selection storage
  TGDoubleHSlider *fTimeSlider;     // Time window selection
  TGStatusBar *fTimeStatus;         // Time window display

  // Core Event Display Controller
  EventDisplay *fEventDisplay;      // Handles data and visualization

public:
  /**
   * @brief Construct a new event display GUI
   * @param fname Path to input data file
   */
  MyMainFrame(const char* fname);
  
  virtual ~MyMainFrame();

  /// @brief Handle window close event
  void DoClose();

  // Event Navigation
  void AdvanceSpill();    // Move to next spill
  void PreviousSpill();   // Return to previous spill

  // Display Configuration
  void ColorbySlice();    // Set color-by-slice mode
  void ColorbyPFP();      // Set color-by-PFP mode
  void DrawCRTHits();     // Toggle CRT hits display
  void DrawFlashes();     // Toggle optical flashes
  void DrawReco();        // Toggle reconstructed objects

  // Cut Management
  void CheckSliceCut();   // Apply selected slice cuts
  void CheckSpillCut();   // Apply selected spill cuts
  void CheckNuSlice();    // Toggle neutrino slice filter

  // Plane Visibility
  void CheckPlane1();     // Toggle Induction 1 plane
  void CheckPlane2();     // Toggle Induction 2 plane
  void CheckPlane3();     // Toggle Collection plane

  // Time Window Control
  void DoSlider();        // Handle time range updates

private:
  /// @brief Update all display status indicators
  void UpdateEventDisplay();
};

#endif // MY_MAIN_FRAME_H
