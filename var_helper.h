

#include "sbnana/CAFAna/Core/Binning.h"
#include "sbnana/CAFAna/Core/Cut.h"
//#include "sbnana/CAFAna/StandardRecord/Proxy/SRProxy.h"
#include "sbnanaobj/StandardRecord/Proxy/SRProxy.h"

#include "sbnana/SBNAna/Vars/NumuVars.h"
#include "sbnana/SBNAna/Vars/NueVars.h"
#include "sbnana/SBNAna/Vars/Vars.h"
#include "sbnana/SBNAna/Vars/TruthVars.h"
#include "sbnana/SBNAna/Cuts/Cuts.h"
#include "sbnana/SBNAna/Cuts/TruthCuts.h"
#include "sbnana/SBNAna/Cuts/NueCuts.h"

#include "TVector3.h"
#include "TLorentzVector.h"
#include "TCanvas.h"
#include "TH1.h"
#include "TLegend.h"
#include "TMath.h"

using namespace ana;

// Populate this vector with your spill variables!
std::vector<SpillVar> spill_vars = {

};

// Populate this vector with your slice variables!
std::vector<Var> slice_vars = {

};
