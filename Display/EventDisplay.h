#ifndef EVENT_DISPLAY_HELPER_H
#define EVENT_DISPLAY_HELPER_H

#include "sbnana/CAFAna/Core/SpectrumLoader.h"
#include "sbnana/CAFAna/Core/Spectrum.h"
#include "sbnana/CAFAna/Core/Binning.h"

#include "TEveManager.h"
#include "TEveEventManager.h"
#include "TEveGeoShape.h"

#include "MultiView.h"

#include <vector>
#include <string>
#include <memory>

class EventDisplay {
public:
    explicit EventDisplay(const char* fname);
    ~EventDisplay();

    // Main interface
    void InitializeDisplay();
    void LoadSpill(size_t spill);
    void DrawAll();

    // Navigation
    void NextSpill();
    void PreviousSpill();

    // Configuration
    void SetColorBySlice();
    void SetColorByPFP();
    void SetPlaneVisibility(unsigned plane, bool visible);
    void SetTimeRange(float min, float max);
    void SetNuSliceFilter(bool enable);

    void ToggleCRTHits(bool enable);
    void ToggleReco(bool enable);
    void ToggleFlashes(bool enable);

    int GetCurrentRun() const;
    int GetCurrentEvent() const;
    std::pair<float, float> GetTimeWindow() const;

    void ApplySliceCuts(const std::vector<int>& cuts);
    void ApplySpillCuts(const std::vector<int>& cuts);

private:
    // Data containers
    struct SpillData {
        struct HitCollection {
            std::vector<double> x, y, z, pfp, slice, plane;
        };
        
        struct CRTHitCollection {
            std::vector<double> x, y, z, time, plane;
        };
        
        struct FlashCollection {
            std::vector<double> x, y, z, time, widthY, widthZ;
        };
        
        struct TrackCollection {
            std::vector<double> startX, startY, startZ;
            std::vector<double> endX, endY, endZ;
            std::vector<double> pfp, slice;
        };
        
        struct ShowerCollection {
            std::vector<double> startX, startY, startZ;
            std::vector<double> dirX, dirY, dirZ;
            std::vector<double> length, angle, pfp, slice;
        };

        HitCollection hits;
        CRTHitCollection crt;
        FlashCollection flashes;
        TrackCollection tracks;
        ShowerCollection showers;
        int run;
        int event;
    };

    // Visualization state
    struct DisplayConfig {
        bool colorBySlice = false;
        bool colorByPFP = false;
        bool planesVisible[3] = {true, true, true};
        bool showCRT = false;
        bool showFlashes = false;
        bool showReco = true;
        bool onlyNuSlice = true;
        float timeMin = -5.0f;
        float timeMax = 5.0f;
    };

    // Data members
    std::vector<SpillData> m_spills;
    size_t m_currentSpill = 0;
    const char* m_filename;
    DisplayConfig m_config;
    int m_detector;

    ana::SpillCut m_combinedSpillCut = ana::kNoSpillCut;

    // TEve components
    TEveManager* m_eve = nullptr;
    MultiView* m_multiView = nullptr;
    TEveGeoShape* m_geometry = nullptr;

    // Private methods
    void LoadData();
    void SetupGeometry();
    void ClearScene();
    
    void DrawTPCHits();
    void DrawCRTHits();
    void DrawFlashes();
    void DrawTracks();
    void DrawShowers();

    int GetColor(int id) const;
    void UpdateViews();

    // Variable trackers to sync data values
    constexpr static int x = 0;
    constexpr static int y = 1;
    constexpr static int z = 2;
    constexpr static int kpfp = 3;
    constexpr static int kslice = 4;
    constexpr static int kplaneid = 5;
    constexpr static int kHitEnd = 6;

    constexpr static int crtx = 0;
    constexpr static int crty = 1;
    constexpr static int crtz = 2;
    constexpr static int crttime = 3;
    constexpr static int crtplane = 4;
    constexpr static int kCRTEnd = 5;

    constexpr static int flashx = 0;
    constexpr static int flashy = 1;
    constexpr static int flashz = 2;
    constexpr static int flashwidthy = 3;
    constexpr static int flashwidthz = 4;
    constexpr static int flashtime = 5;
    constexpr static int kFlashEnd = 6;

    constexpr static int trackstartx = 0;
    constexpr static int trackstarty = 1;
    constexpr static int trackstartz = 2;
    constexpr static int trackendx = 3;
    constexpr static int trackendy = 4;
    constexpr static int trackendz = 5;
    constexpr static int trackpfp = 6;
    constexpr static int trackslice = 7;
    constexpr static int kTrackEnd = 8;

    constexpr static int showerstartx = 0;
    constexpr static int showerstarty = 1;
    constexpr static int showerstartz = 2;
    constexpr static int showerdirx = 3;
    constexpr static int showerdiry = 4;
    constexpr static int showerdirz = 5;
    constexpr static int showerlength = 6;
    constexpr static int showerangle = 7;
    constexpr static int showerpfp = 8;
    constexpr static int showerslice = 9;
    constexpr static int kShowerEnd = 10;
};

#endif // EVENT_DISPLAY_HELPER_H
