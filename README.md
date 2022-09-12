# CAFDisplay
3D event display to work with CAF files in the SBN collaboration.

## Introduction
This is a lightweight event display that runs on caf and flatcaf files from events generated in the ICARUS LArTPCs. The purpose of this event display is to provide a user-friendly debugging tool for neutrino/cosmic ray analyses and reconstruction efforts. It contains a click-and-drag rotatable display, selectable drawing options, and buttons for advancing to either the next spill in the CAF file or the previous one.

The event display utilizes recob::Hit and recob::SpacePoint information added to the CAFs in [SBNSoftware/sbnanaobj](https://github.com/SBNSoftware/sbnanaobj/tree/develop) version v09_19_02 and [SBNSoftware/sbncode](https://github.com/SBNSoftware/sbncode/tree/develop) version v09_58_02.

Some data events from run 7418 are available for you to check out here: `/icarus/app/users/rhowell/SRHits/test.flat.caf.root`

N.B. You may need to run the bash command `stty sane` after closing the GUI window. There's a bug with ROOT applications closing that sometimes you won't see typing or updates on the command line. This will fix that. 

## How to Use
### Base Use
1. On an ICARUS gpvm, go to your working area and clone this repository `git clone https://github.com/rhowell42/CAFDisplay.git`
3. Setup the relevant SBNSoftware/sbnana version `setup sbnana -v v09_58_02 -q e20:prof` N.B. You need version v09_58_02 or later
4. Start your vncserver if you haven't already. Instructions for doing this [here](https://sbnsoftware.github.io/sbndcode_wiki/Viewing_events_remotely_with_VNC.html) if you haven't done this before.
5. In that same working area, run `cafe event_display.C {path/to/your/file.caf.root}`
6. ???
7. Profit


### Generate Your Own CAFs
You will need your own copy of [cafmakerjob_icarus.fcl](https://github.com/SBNSoftware/icaruscode/blob/develop/fcl/caf/cafmakerjob_icarus.fcl) ([cafmakerjob_icarus_data.fcl](https://github.com/SBNSoftware/icaruscode/blob/develop/fcl/caf/cafmakerjob_icarus_data.fcl) if you want to look at data). There you will add the line `cafmaker.FillHits: true` which will fill all of the SRHit and SRSpacePoint information needed to use the event display. Then run `lar -c cafmakerjob_icarus{_data}.fcl` as usual.

**WARNING: These CAF files will be an order of magnitude larger in size than the normal CAFs. 100 data events will be ~300 MB in size.**

### User Cuts
Add the slice and spill cuts you would like to optionally apply to `cut_helper.h`; there are two vectors, one for slice cuts and the other for spill cuts. Do not change the name of these vectors, but add your cuts to them in the same way the `kNoCut` are added. I would recommened adding an `#include your_cuts.h` statement to keep `cut_helper.h` visually clean, where `your_cuts.h` is where you actually define the cuts you use for you selection. 

### Drawing Options
- Color by Slice
  - Color code the SRSpacePoints based one which SRSlice they belong to.
- Color by PFP
  - Color code the SRSpacePoints based one which SRPFP they are reconstructed into

### Plotting Options
- Only Plot Nu Slice
  - Redraw the scene to only show slices that aren't identified by Pandora as "clear cosmic." Checking or unchecking this box will rerun the spectrum loader, so give it some time.
- Apply Slice Cuts
  - Redraw the scene to only show slices that pass a vector of user defined slice cuts.
- Apply Spill Cuts
  - Redraw the scene, return to the base spill of the CAF file, and only plot spills that pass a vector of user defined slice cuts. This will restart the event loop and plot the first spill sequentially that passes these cuts.
  
### Event Navigation
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

~~The event display utilizes recob::Hit and recob::SpacePoint information added to the CAFs in two feature branches, both named feature/rh_SRHits, in [SBNSoftware/sbnanaobj](https://github.com/SBNSoftware/sbnanaobj/tree/feature/rh_SRHits) and [SBNSoftware/sbncode](https://github.com/SBNSoftware/sbncode/tree/feature/rh_SRHits). In order to use the event display, you will need a CAF file generated with this information added, and you will need a local build of SBNSoftware/sbnanaobj with my feature branch feature/rh_SRHits and a base local build of [SBNSoftware/sbnana](https://github.com/SBNSoftware/sbnana/tree/develop). SBNSoftware/sbnana is necessary until these changes are absorbed into the develop branch; otherwise you need the local build for sbnana/CAFAna/Core/FileReducer.cxx and some other files to be consistent with sbnanaobj.~~
