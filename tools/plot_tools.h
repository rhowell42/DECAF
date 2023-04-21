
#include <iostream>
#include <vector>
#include <algorithm>

std::string fname;

bool ColorByPFPs = false;
bool ColorBySlice = true;

bool DrawPlane1 = true;
bool DrawPlane2 = true;
bool DrawPlane3 = true;

std::vector<std::vector<double>> X;
std::vector<std::vector<double>> Y;
std::vector<std::vector<double>> Z;

std::vector<std::vector<double>> PFPID;
std::vector<std::vector<double>> SliceID;
std::vector<std::vector<double>> PlaneID;

std::vector<int> run;
std::vector<int> event;

std::vector<double> plotvars;

std::vector<int> spillCutIndices;

const int x = 0;
const int y = 1;
const int z = 2;
const int kpfp = 3;
const int kslice = 4;
const int kplaneid = 5;
const int kEnd = 6;

MultiView* gMultiView = 0;

int spill = 0; 
int nSpills = 0;

void GetSpectrumSelection();

template<typename T>
std::vector<int> findItems(std::vector<T> const &v, int target) {
    std::vector<int> indices;
    auto it = v.begin();
    while ((it = std::find_if(it, v.end(), [&] (T const &e) { return int(e) == target; }))
        != v.end())
    {
        indices.push_back(std::distance(v.begin(), it));
        it++;
    }
    return indices;
}
