# DeCAF
3D event display to work with CAF files in the SBN collaboration.

## Introduction
This is a lightweight event display that runs on caf and flatcaf files from events generated in the ICARUS LArTPCs. The purpose of this event display is to provide a user-friendly debugging tool for neutrino/cosmic ray analyses and reconstruction efforts. It contains a click-and-drag rotatable, mouse wheel zoom and pan 3D display, selectable hit coloring options, and buttons for advancing to either the next spill in the CAF file or the previous one.

The event display utilizes recob::Hit and recob::SpacePoint information added to the CAFs in [SBNSoftware/sbnanaobj](https://github.com/SBNSoftware/sbnanaobj/tree/develop) version v09_19_02 and [SBNSoftware/sbncode](https://github.com/SBNSoftware/sbncode/tree/develop) version v09_58_02.

Some data events from run 7418 are available for you to check out here: `/icarus/app/users/rhowell/SRHits/test.flat.caf.root`

## How to Use
### Base Use
1. On an ICARUS gpvm, go to your working area and clone this repository `git clone https://github.com/rhowell42/DeCAF.git`
3. Setup the relevant SBNSoftware/sbnana version `setup sbnana -v v09_58_02 -q e20:prof` 
   -N.B. You need version v09_58_02 or later. `scripts/setup_online.sh` does this for you
4. Start your vncserver if you haven't already. Instructions for doing this [here](https://sbnsoftware.github.io/sbndcode_wiki/Viewing_events_remotely_with_VNC.html) if you haven't done this before.
5. Add any slice and/or spill cuts you might want to select in the respective vectors in `cut_helper.h`
5. In that same working area, run `cafe event_display.C {path/to/your/file.caf.root}`
6. ???
7. Profit


### Generate Your Own CAFs
You will need your own copy of [cafmakerjob_icarus.fcl](https://github.com/SBNSoftware/icaruscode/blob/develop/fcl/caf/cafmakerjob_icarus.fcl) ([cafmakerjob_icarus_data.fcl](https://github.com/SBNSoftware/icaruscode/blob/develop/fcl/caf/cafmakerjob_icarus_data.fcl) if you want to look at data). There you will add the line `cafmaker.FillHits: true` which will fill all of the SRHit and SRSpacePoint information needed to use the event display. Then run `lar -c cafmakerjob_icarus{_data}.fcl` as usual.

`scripts/cafmakerjob_icarus{_data}.fcl` already has these lines in them, so feel free to use those to generate the CAFs.

**WARNING: These CAF files will be an order of magnitude larger in size than the normal CAFs. 100 data events will be ~300 MB in size.**

### User Cuts
Add the slice and spill cuts you would like to optionally apply to `cut_helper.h`; there are two vectors, one for slice cuts and the other for spill cuts. Do not change the name of these vectors, but add your cuts to them in the same way the `kNoCut` are added. I would recommened adding an `#include your_cuts.h` statement to keep `cut_helper.h` visually clean, where `your_cuts.h` is where you actually define the cuts you use for you selection. These cuts will automatically be added to a drop-down text box in the event display where you can select any arbitrary combination of them to apply to the events displayed.

### Drawing Options
- Color by Slice
  - Color code the SRSpacePoints based one which SRSlice they belong to.
- Color by PFP
  - Color code the SRSpacePoints based one which SRPFP they are reconstructed into

### Plotting Options
- Only Plot Nu Slice
  - Redraw the scene to only show slices that aren't identified by Pandora as "clear cosmic." Checking or unchecking this box will rerun the spectrum loader, so give it some time.
- Apply Slice Cuts
  - Select the slice cuts you would like to apply in the drop-down text box, and hit the `Apply Slice Cuts` button. This will take some time as it reruns the spectrum loader.
- Apply Spill Cuts
  - Select the slice cuts you would like to apply in the drop-down text box, and hit the `Apply Spill Cuts` button. This will take some time as it reruns the spectrum loader.
  
### Event Navigation
- Next Spill
  - Draw the sequentially next spill in the CAF file. If Apply Spill Cuts is checked, the next spill will be the one that meets those spill cut requirements.
- Previous Spill
  - Draw the sequentially previous spill in the CAF file. If Apply Spill Cuts is checked, the next spill will be the one that meets those spill cut requirements.
