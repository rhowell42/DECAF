#define GET_VARIABLE_NAME(Variable)(#Variable)

#include "guitools/GUI.h"
#include "guitools/MultiView.h"
#include "tools/plot_tools.h"
#include "TFile.h"
#include "TChain.h"
#include "TTree.h"
#include "TTreeReader.h"
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

  TChain *myChain = new TChain("caloskimE/TrackCaloSkim");

  TTreeReader myReader(myChain); 

// Declare the variables we want to read from the data files
TTreeReaderArray<float>plane0_x(myReader, "trk.hits0.h.sp.x");
TTreeReaderArray<float>plane1_x(myReader, "trk.hits1.h.sp.x");
TTreeReaderArray<float>plane2_x(myReader, "trk.hits2.h.sp.x");
TTreeReaderArray<float>plane0_y(myReader, "trk.hits0.h.sp.y");
TTreeReaderArray<float>plane1_y(myReader, "trk.hits1.h.sp.y");
TTreeReaderArray<float>plane2_y(myReader, "trk.hits2.h.sp.y");
TTreeReaderArray<float>plane0_z(myReader, "trk.hits0.h.sp.z");
TTreeReaderArray<float>plane1_z(myReader, "trk.hits1.h.sp.z");
TTreeReaderArray<float>plane2_z(myReader, "trk.hits2.h.sp.z");
TTreeReaderValue<float>start_x(myReader, "trk.start.x");
TTreeReaderValue<float>start_y(myReader, "trk.start.y");
TTreeReaderValue<float>start_z(myReader, "trk.start.z");
TTreeReaderValue<float>end_x(myReader, "trk.end.x");
TTreeReaderValue<float>end_y(myReader, "trk.end.y");
TTreeReaderValue<float>end_z(myReader, "trk.end.z");
//TTreeReaderValue<int>run(myReader, "meta.run");
//TTreeReaderValue<int>event(myReader, "meta.evt");

Int_t event_id = 0;

void LoadReco(){

    auto top = gEve->GetCurrentEvent();
    std::list < TEveElement *> children;
    top->FindChildren(children, "Reco");
    for (auto  const & child: children) {
        if (child) {
            child->Destroy();
        }
    }

    auto line = new TEveLine;
    double sx = *start_x;
    double sy = *start_y;
    double sz = *start_z;
    double ex = *end_x;
    double ey = *end_y;
    double ez = *end_z;
    line->SetNextPoint(sx, sy, sz);
    line->SetNextPoint(ex, ey, ez);
    line->SetMainColor(4);
    line->SetMainTransparency(25);
    line->SetLineWidth(5);
    line->SetElementName("Reco");

    gEve->AddElement(line);

    top = gEve->GetCurrentEvent();

    gMultiView->DestroyEventRPhi();
    gMultiView->ImportEventRPhi(top);

    gMultiView->DestroyEventRhoZ();
    gMultiView->ImportEventRhoZ(top);
    gEve->Redraw3D(kFALSE, kTRUE);
} 

void LoadTPCHits() {
    auto top = gEve->GetCurrentEvent();
    std::list < TEveElement *> children;
    top->FindChildren(children, "TPC Hits");
    for (auto  const & child: children) {
        if (child) {
            child->Destroy();
        }
    }
    auto marker0 = new TEvePointSet();
    marker0->SetOwnIds(kTRUE);

    if (DrawPlane1) {
        for (size_t e=0; e < plane0_x.GetSize(); e++) {
            marker0->SetNextPoint(plane0_x[e],plane0_y[e],plane0_z[e]);
            marker0->SetPointId(new TNamed(Form("Point %zu Plane 0", e), ""));
        }

        marker0->SetMarkerSize(.4);
        marker0->SetMarkerStyle(8);
        marker0->SetMainColor(2);
        marker0->SetElementName("TPC Hits");
        gEve->AddElement(marker0);
    }

    auto marker1 = new TEvePointSet();
    marker1->SetOwnIds(kTRUE);

    if (DrawPlane2) {
        for (size_t e=0; e < plane1_x.GetSize(); e++) {
            marker1->SetNextPoint(plane1_x[e],plane1_y[e],plane1_z[e]);
            marker1->SetPointId(new TNamed(Form("Point %zu Plane 1", e), ""));
        }

        marker1->SetMarkerSize(.4);
        marker1->SetMarkerStyle(8);
        marker1->SetMainColor(3);
        marker1->SetElementName("TPC Hits");
        gEve->AddElement(marker1);
    }

    auto marker2 = new TEvePointSet();
    marker2->SetOwnIds(kTRUE);

    if (DrawPlane3) {
        for (size_t e=0; e < plane2_x.GetSize(); e++) {
            marker2->SetNextPoint(plane2_x[e],plane2_y[e],plane2_z[e]);
            marker2->SetPointId(new TNamed(Form("Point %zu Plane 2", e), ""));
        }

        marker2->SetMarkerSize(.4);
        marker2->SetMarkerStyle(8);
        marker2->SetMainColor(5);
        marker2->SetElementName("TPC Hits");
        gEve->AddElement(marker2);
    }
    top = gEve->GetCurrentEvent();

    gMultiView->DestroyEventRPhi();
    gMultiView->ImportEventRPhi(top);

    gMultiView->DestroyEventRhoZ();
    gMultiView->ImportEventRhoZ(top);
    gEve->Redraw3D(kFALSE, kTRUE);
    }

    void doAdvanceSpill(float min, float max) {
        if (event_id < myReader.GetEntries() - 1) {
            ++event_id;
            myReader.SetEntry(event_id);
            myReader.Next();
            //eventstring = event;
            //runstring = run;
        if (PlotReco) {
            LoadReco();
        }
            LoadTPCHits();
        }
    }
    void doPreviousSpill(float min, float max) {
        if (event_id > 0) {
            --event_id;
            myReader.SetEntry(event_id);
            myReader.Next();
            //eventstring = event;
            //runstring = run;
        if (PlotReco) {
            LoadReco();
        }
            LoadTPCHits();
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
            std::cout<<"CRT Hits not supported :("<<std::endl;//LoadCRTHits(min, max);
        } else {
            auto top = gEve->GetCurrentEvent();
            auto child = top->FindChild("CRT Hits");
            if (child) {
                child->Destroy();
            }
        }
    }
    void doDrawFlashes(bool pressed, float min, float max) {
        std::cout<<"Flashes not supported."<<std::endl;
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
        //useSliceCuts = true;
        //sliceCutIndices = cut_indices;
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
        //useSpillCuts = true;
        //spillCutIndices = cut_indices;
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
        //onlyNuSlice = pressed;
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
            std::cout<<"CRT Hits not supported :("<<std::endl;//LoadCRTHits(min, max);
        }
        if (PlotFlashes) {
            std::cout<<"Flashes not supported :("<<std::endl;//LoadFlashes(min, max);
        }
    }

    void calibration_display() {
        gSystem->Load("libGeom");

        TEveManager::Create();

        TEveGeoShape *gentle_geom = 0;
        const int detector = 2;

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

        std::string data_or_mc = "data";

  int which_10th = 9;

  if (data_or_mc == "data") {
    vector<string> file_list = make_directory_list("scripts/bnbfiles6819_8459.txt");
    //vector<string> file_list = make_directory_list("/icarus/app/users/gputnam/calib-ana/bnbfiles.list");
    int file_list_size = file_list.size();
    int file_list_10th = file_list_size / 10;  
    if (which_10th < 9) {
      for (int i=0; i<file_list_10th; i++) {
        const char *file = file_list[which_10th*file_list_10th + i].c_str();
        myChain->Add(file);
    }}
    else {
      if (which_10th == 9) {
        int max_i = file_list_10th + (file_list_size % 10);
        for (int i=0; i<max_i; i++) {
          const char *file = file_list[which_10th*file_list_10th + i].c_str();
          myChain->Add(file);
      }}
      else {
        cout<<"which_10th must be an integer >= 0 and < 10"<<endl;
        return;
  }}}

  else if (data_or_mc == "mc") {
    vector<string> file_list = make_directory_list("scripts/MCdirectories.txt");
    for (int i=0; i<file_list.size(); i++) {
       const char *file = ("/pnfs/icarus/scratch/users/gputnam/MCP2022G/cosmic/"+file_list[i]+"/Supplemental*stage1.root").c_str();
       myChain->Add(file);
  }}
        myReader.SetEntry(event_id);
        myReader.Next();
        //eventstring = event;
        //runstring = run;

        LoadReco();
        LoadTPCHits();
  gEve->Redraw3D(kTRUE); // Reset camera after the first event has been shown.
}
