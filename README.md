# CAFDisplay
3D event display to work with CAF files in the SBN collaboration.

## Introduction
This is a lightweight event display that runs on caf and flatcaf files from events generated in the ICARUS LArTPCs. The purpose of this event display is to provide a user-friendly debugging tool for neutrino/cosmic ray analyses and reconstruction efforts. It contains a click-and-drag rotatable display, selectable drawing options, and buttons for advancing to either the next spill in the CAF file or the previous one.

~~The event display utilizes recob::Hit and recob::SpacePoint information added to the CAFs in two feature branches, both named feature/rh_SRHits, in [SBNSoftware/sbnanaobj](https://github.com/SBNSoftware/sbnanaobj/tree/feature/rh_SRHits) and [SBNSoftware/sbncode](https://github.com/SBNSoftware/sbncode/tree/feature/rh_SRHits). In order to use the event display, you will need a CAF file generated with this information added, and you will need a local build of SBNSoftware/sbnanaobj with my feature branch feature/rh_SRHits and a base local build of [SBNSoftware/sbnana](https://github.com/SBNSoftware/sbnana/tree/develop). SBNSoftware/sbnana is necessary until these changes are absorbed into the develop branch; otherwise you need the local build for sbnana/CAFAna/Core/FileReducer.cxx and some other files to be consistent with sbnanaobj.~~

The event display utilizes recob::Hit and recob::SpacePoint information added to the CAFs in [SBNSoftware/sbnanaobj](https://github.com/SBNSoftware/sbnanaobj/tree/develop) version v09_19_02 and [SBNSoftware/sbncode](https://github.com/SBNSoftware/sbncode/tree/develop) version v09_58_02. 

Some data events from run 7418 are available for you to check out here: `/icarus/app/users/rhowell/SRHits/test.flat.caf.root`

## How to Use
### Base Use
1. On an ICARUS gpvm, go to your working area and clone this repository `git clone https://github.com/rhowell42/CAFDisplay.git`
2. Start your vncserver if you haven't already.
3. In that same working area, run `cafe event_display.C {path/to/your/file.caf.root}`
4. ???
5. Profit

### User Cuts
Add the slice and spill cuts you would like to optionally apply to `cut_helper.h`; there are two vectors, one for slice cuts and the other for spill cuts. Do not change the name of these vectors, but add your cuts to them in the same way the `kNoCut` are added. I would recommened adding an `#include your_cuts.h` statement to keep `cut_helper.h` visually clean, where `your_cuts.h` is where you actually define the cuts you use for you selection. 

### Drawing Options
- Color by Slice
  - Color code the SRSpacePoints based one which SRSlice they belong to.
- Color by PFP
  - Color code the SRSpacePoints based one which PFP they are reconstructed into

### Plotting Options
- Only Plot Nu Slice
  - Redraw the scene to only show slices that aren't identified by Pandora as "clear cosmic." Checking or unchecking this box will rerun the spectrum loader, so give it some time.
- Apply Slice Cuts
  - Redraw the scene to only show slices that pass a vector of user defined slice cuts.
- Apply Spill Cuts
  - Redraw the scene, return to the base spill of the CAF file, and only plot spills that pass a vector of user defined slice cuts.
  
### Spill Navigation
- Next Spill
  - Draw the sequentially next spill in the CAF file. If Apply Spill Cuts is checked, the next spill will be the one that meets those spill cut requirements.
- Previous Spill
  - Draw the sequentially previous spill in the CAF file. If Apply Spill Cuts is checked, the next spill will be the one that meets those spill cut requirements.

# TODO
- Add ability to zoom in and pan the display
- Add color option to color based on recob::Hit ADC Integral
- Add geometric objects to represent ICARUS detector
  - Cathode planes
  - Cryostat box/wire planes
