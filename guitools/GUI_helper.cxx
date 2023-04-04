#include <iostream>
#include <iterator>
#include <fstream>
#include <vector>
#include <algorithm> // for std::copy

std::vector<std::string> SliceCuts() {
    std::vector<std::string> cuts;
    std::string line;
    std::ifstream infile("cut_helper.h");

    bool startofcuts = 0;
    bool endofcuts = 0;
    while (std::getline(infile, line) && !endofcuts) {
        std::istringstream iss(line);
        line.erase(std::remove_if(line.begin(), line.end(), ::isspace),line.end());
        if (startofcuts) { 
            endofcuts = line.find("};") != string::npos;
        }
        if (endofcuts) { continue; }
        if (startofcuts) {
            line.erase(std::remove(line.begin(), line.end(), ','), line.end());
            cuts.push_back(line);
        }
        if (!startofcuts) { startofcuts = line.find("slice_cuts={") != string::npos; }
    }
    return(cuts);
}

std::vector<std::string> SpillCuts() {
    std::vector<std::string> cuts;
    std::string line;
    std::ifstream infile("cut_helper.h");

    bool startofcuts = 0;
    bool endofcuts = 0;
    while (std::getline(infile, line) && !endofcuts) {
        std::istringstream iss(line);
        line.erase(std::remove_if(line.begin(), line.end(), ::isspace),line.end());
        if (startofcuts) { 
            endofcuts = line.find("};") != string::npos;
        }
        if (endofcuts) { continue; }
        if (startofcuts) {
            line.erase(std::remove(line.begin(), line.end(), ','), line.end());
            cuts.push_back(line);
        }
        if (!startofcuts) { startofcuts = line.find("spill_cuts={") != string::npos; }
    }
    return(cuts);
}
