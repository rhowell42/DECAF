#include "TEveGeoShape.h"
#include "TEvePointSet.h"
#include "TEveLine.h"
#include "TEveBoxSet.h"
#include "TMath.h"
#include "TFile.h"

#include "EventDisplay.h"
#include "DisplayVars.h"


// Constructor/Destructor
EventDisplay::EventDisplay(const char* fname) 
    : m_filename(fname) 
{
    LoadData();
    InitializeDisplay();
}

EventDisplay::~EventDisplay() {
    delete m_multiView;
}

// Initialize the Display
void EventDisplay::InitializeDisplay() {
    // TEve initialization
    m_eve = TEveManager::Create();
    SetupGeometry();

    // Multi-view setup
    m_multiView = new MultiView;
    m_multiView->SetDepth(-10);
    m_multiView->ImportGeomRPhi(m_geometry);
    m_multiView->ImportGeomRhoZ(m_geometry);
    m_multiView->SetDepth(0);

    std::string detName = (m_detector == 1) ? "SBND" : "ICARUS";
    std::string managerName = detName+" CAF Event";
    m_eve->AddEvent(new TEveEventManager("Event", managerName.c_str()));
}

// Load the geometry
void EventDisplay::SetupGeometry() {
    TEveGeoShape *gentle_geom = 0;

    if (m_detector == 1) {
        auto file = TFile::Open("geometry/simple_SBND_geom.root");
        if (!file) return;
        auto gse = (TEveGeoShapeExtract *) file->Get("Gentle");
        gentle_geom = TEveGeoShape::ImportShapeExtract(gse, 0);
        file->Close();
    }
    else if (m_detector == 2) {
        auto file = TFile::Open("geometry/simple_ICARUS_geom.root");
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

    m_geometry = gentle_geom;
    m_eve->AddGlobalElement(m_geometry);
}

// Data loading
void EventDisplay::LoadData() {
    SpectrumLoader loader(m_filename);
    const Binning bins = Binning::Simple(1, 0, 1);
    
    const SpillVar kFindEvents([this](const caf::SRSpillProxy *sr)->int {
        SpillData sd;

        // Process detector
        m_detector = kDETECTOR(sr);

        // Process hits
        const auto& hitvars = kHITVARS(sr);
        for(size_t i = 0; i < hitvars.size(); i += kHitEnd) {
            sd.hits.x.push_back(hitvars[i + x]);
            sd.hits.y.push_back(hitvars[i + y]);
            sd.hits.z.push_back(hitvars[i + z]);
            sd.hits.pfp.push_back(hitvars[i + kpfp]);
            sd.hits.slice.push_back(hitvars[i + kslice]);
            sd.hits.plane.push_back(hitvars[i + kplaneid]);
        }

        // Process tracks
        const auto& trackvars = kTRACKVARS(sr);
        for(size_t i = 0; i < trackvars.size(); i += kTrackEnd) {
            sd.tracks.startX.push_back(trackvars[i + trackstartx]);
            sd.tracks.startY.push_back(trackvars[i + trackstarty]);
            sd.tracks.startZ.push_back(trackvars[i + trackstartz]);
            sd.tracks.endX.push_back(trackvars[i + trackendx]);
            sd.tracks.endY.push_back(trackvars[i + trackendy]);
            sd.tracks.endZ.push_back(trackvars[i + trackendz]);
            sd.tracks.pfp.push_back(trackvars[i + trackpfp]);
            sd.tracks.slice.push_back(trackvars[i + trackslice]);
        }

        //Process showers
        const auto& showervars = kSHOWERVARS(sr);
        for(size_t i = 0; i < showervars.size(); i += kShowerEnd) {
            sd.showers.startX.push_back(showervars[i + showerstartx]);
            sd.showers.startY.push_back(showervars[i + showerstarty]);
            sd.showers.startZ.push_back(showervars[i + showerstartz]);
            sd.showers.dirX.push_back(showervars[i + showerdirx]);
            sd.showers.dirY.push_back(showervars[i + showerdiry]);
            sd.showers.dirZ.push_back(showervars[i + showerdirz]);
            sd.showers.length.push_back(showervars[i + showerlength]);
            sd.showers.angle.push_back(showervars[i + showerangle]);
            sd.showers.pfp.push_back(showervars[i + showerpfp]);
            sd.showers.slice.push_back(showervars[i + showerslice]);
        }

        //Process crt hits
        const auto& crtvars = kCRTVARS(sr);
        for(size_t i = 0; i < crtvars.size(); i += kCRTEnd) {
            sd.crt.x.push_back(crtvars[i + crtx]);
            sd.crt.y.push_back(crtvars[i + crty]);
            sd.crt.z.push_back(crtvars[i + crtz]);
            sd.crt.time.push_back(crtvars[i + crttime]);
            sd.crt.plane.push_back(crtvars[i + crtplane]);
        }

        //Process op flashes
        const auto& flashvars = kOPVARS(sr);
        for(size_t i = 0; i < flashvars.size(); i += kFlashEnd) {
            sd.flashes.x.push_back(flashvars[i + flashx]);
            sd.flashes.y.push_back(flashvars[i + flashy]);
            sd.flashes.z.push_back(flashvars[i + flashz]);
            sd.flashes.time.push_back(flashvars[i + flashtime]);
            sd.flashes.widthY.push_back(flashvars[i + flashwidthy]);
            sd.flashes.widthZ.push_back(flashvars[i + flashwidthz]);
        }

        sd.run = kRun(sr);
        sd.event = kEvt(sr);
        m_spills.push_back(sd);
        return 1.0;
    });

    Spectrum sFindSpill("", bins, loader, kFindEvents, m_combinedSpillCut, kSpillUnweighted);
    loader.Go();
}

// Cumulatively apply spill cuts
void EventDisplay::ApplySpillCuts(const std::vector<int>& cuts) {
  for (auto cut: cuts) {
    m_combinedSpillCut = m_combinedSpillCut && spill_cuts[cut];
  }
  LoadData();
}

// Cumulatively apply slice cuts

void EventDisplay::ApplySliceCuts(const std::vector<int>& cuts) {
    std::cout<<"Slice cuts are not supported yet"<<std::endl;
}


// Visualization methods
void EventDisplay::DrawTracks() {
    const auto& tracks = m_spills[m_currentSpill].tracks;
    auto* lines = new TEveLine("Tracks");
    
    for(size_t i = 0; i < tracks.startX.size(); ++i) {
        lines->SetNextPoint(tracks.startX[i], tracks.startY[i], tracks.startZ[i]);
        lines->SetNextPoint(tracks.endX[i], tracks.endY[i], tracks.endZ[i]);
        
        if(m_config.colorBySlice) {
            lines->SetLineColor(tracks.slice[i]);
        } else if(m_config.colorByPFP) {
            lines->SetLineColor(abs(tracks.pfp[i]));
        }
    }
    
    lines->SetLineWidth(2);
    m_eve->AddElement(lines);
}

void EventDisplay::DrawTPCHits() {
    auto* hitPoints = new TEvePointSet("TPC Hits");
    hitPoints->SetMarkerStyle(8);
    hitPoints->SetMarkerSize(0.4);

    const auto& hits = m_spills[m_currentSpill].hits;
    for(size_t i = 0; i < hits.x.size(); ++i) {
        // Skip hits from hidden planes
        if(!m_config.planesVisible[static_cast<int>(hits.plane[i])]) continue;

        hitPoints->SetNextPoint(hits.x[i], hits.y[i], hits.z[i]);
        
        // Set color based on configuration
        if(m_config.colorBySlice) {
            hitPoints->SetMarkerColor(GetColor(hits.slice[i]));
        } else if(m_config.colorByPFP) {
            hitPoints->SetMarkerColor(GetColor(hits.pfp[i]));
        }
    }
    
    m_eve->AddElement(hitPoints);
}

void EventDisplay::DrawCRTHits() {
    auto* crtPoints = new TEvePointSet("CRT Hits");
    crtPoints->SetMarkerStyle(4);
    crtPoints->SetMarkerColor(kRed);
    crtPoints->SetMarkerSize(1.0);

    const auto& crtHits = m_spills[m_currentSpill].crt;
    for(size_t i = 0; i < crtHits.x.size(); ++i) { 
        if(crtHits.time[i] < m_config.timeMin || crtHits.time[i] > m_config.timeMax) continue;
        crtPoints->SetNextPoint(crtHits.x[i], crtHits.y[i], crtHits.z[i]);
    }
    
    m_eve->AddElement(crtPoints);
}

void EventDisplay::DrawFlashes() {
    auto* flashBoxes = new TEveBoxSet("Flashes");
    flashBoxes->Reset(TEveBoxSet::kBT_AABox, true, 64);
    flashBoxes->SetMainColor(kYellow);
    flashBoxes->SetMainTransparency(70);

    const auto& flashes = m_spills[m_currentSpill].flashes;
    for(size_t i = 0; i < flashes.x.size(); ++i) {
        if(flashes.time[i] < m_config.timeMin || flashes.time[i] > m_config.timeMax) continue;

        const float dx = 1.0f;  // Fixed thickness in X
        const float dy = flashes.widthY[i] / 2.0f;
        const float dz = flashes.widthZ[i] / 2.0f;
        
        flashBoxes->AddBox(flashes.x[i], flashes.y[i], flashes.z[i], dx, dy, dz);
    }
    
    m_eve->AddElement(flashBoxes);
}

void EventDisplay::DrawShowers() {
    auto* showers = new TEveBoxSet("Showers");
    showers->Reset(TEveBoxSet::kBT_Cone, true, 64);
    
    const auto& showerData = m_spills[m_currentSpill].showers;
    for(size_t i = 0; i< showerData.startX.size(); ++i) {
        TEveVector pos(showerData.startX[i], showerData.startY[i], showerData.startZ[i]);
        TEveVector dir(showerData.dirX[i], showerData.dirY[i], showerData.dirZ[i]);
        dir *= showerData.length[i];
        const double radius = TMath::Tan(showerData.angle[i]) * showerData.length[i];

        showers->AddCone(pos, dir, radius);
        
        // Set color based on configuration
        if(m_config.colorBySlice) {
            showers->DigitColor(GetColor(showerData.slice[i]));
        } else if(m_config.colorByPFP) {
            showers->DigitColor(GetColor(showerData.pfp[i]));
        }
    }
    
    m_eve->AddElement(showers);
}

void EventDisplay::DrawAll() {
    m_eve->GetCurrentEvent()->DestroyElements();

    DrawTPCHits();
    DrawTracks();
    DrawShowers();

    if (m_config.showReco) {
        DrawTracks();
        DrawShowers();
    }

    if (m_config.showCRT) { DrawCRTHits(); }
    if (m_config.showFlashes) { DrawFlashes(); }

    UpdateViews();
}

// Control configurations
void EventDisplay::ToggleFlashes(bool enable) {
    m_config.showFlashes = enable;
    DrawAll();
}

void EventDisplay::ToggleCRTHits(bool enable) {
    m_config.showCRT = enable;
    DrawAll();
}

void EventDisplay::ToggleReco(bool enable) {
    m_config.showReco = enable;
    DrawAll();
}

// View update helper
void EventDisplay::UpdateViews() {
    auto* eventScene = m_eve->GetCurrentEvent();
    
    m_multiView->DestroyEventRPhi();
    m_multiView->ImportEventRPhi(eventScene);
    
    m_multiView->DestroyEventRhoZ();
    m_multiView->ImportEventRhoZ(eventScene);
    
    m_eve->Redraw3D(kFALSE, kTRUE);
}

// Color management
int EventDisplay::GetColor(int id) const {
    // Use ROOT's color wheel with 16-step repeating pattern
    const int baseColors[] = {kRed, kGreen, kBlue, kYellow, kMagenta, kCyan, kOrange};
    const int nColors = sizeof(baseColors)/sizeof(baseColors[0]);
    return baseColors[abs(id) % nColors];
}

// Navigation
void EventDisplay::NextSpill() {
    if(m_currentSpill < m_spills.size() - 1) {
        ++m_currentSpill;
        DrawAll();
    }
}

void EventDisplay::PreviousSpill() {
    if(m_currentSpill < m_spills.size() - 1) {
        --m_currentSpill;
        DrawAll();
    }
}

// Configuration
void EventDisplay::SetColorBySlice() {
    m_config.colorBySlice = true;
    m_config.colorByPFP = false;
    DrawAll();
}

void EventDisplay::SetColorByPFP() {
    m_config.colorByPFP = true;
    m_config.colorBySlice = false;
    DrawAll();
}

void EventDisplay::SetNuSliceFilter(bool enable) {
    m_config.onlyNuSlice = true;
    LoadData(); 
}

void EventDisplay::SetTimeRange(float min, float max) {
    m_config.timeMin = min;
    m_config.timeMax = max;
}

void EventDisplay::SetPlaneVisibility(unsigned int id, bool enable) {
    m_config.planesVisible[id] = enable;
    LoadData();
}

// Getter Functions
int EventDisplay::GetCurrentRun() const { 
    return this->m_spills[this->m_currentSpill].run;
}

int EventDisplay::GetCurrentEvent() const {
    return this->m_spills[this->m_currentSpill].event;
}

std::pair<float, float> EventDisplay::GetTimeWindow() const {
    return std::make_pair(this->m_config.timeMin,this->m_config.timeMax);
}

