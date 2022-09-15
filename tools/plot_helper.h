#include "TH3.h"

std::string fname;
std::string title = "3dframe";
TH3F *frame3d = new TH3F(title.c_str(),title.c_str(),1,-400,400,1,-1000,1000,1,-200,200);  

bool ColorByPFPs = false;
bool ColorBySlice = true;

std::vector<double> X;
std::vector<double> Y;
std::vector<double> Z;

std::vector<double> PFPID;
std::vector<double> SliceID;

unsigned int spill = 0; 
unsigned int nSpills = 10;
