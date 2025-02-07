#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <string> 

// Helper function to extract cut definitions from cut_helper.h
static std::vector<std::string> ExtractCuts(const std::string& cut_identifier) {
    std::vector<std::string> cuts;
    std::ifstream infile("cut_helper.h");
    std::string line;

    bool in_cut_section = false;
    bool end_of_section = false;

    while (std::getline(infile, line) && !end_of_section) {
        // Remove all whitespace for easier pattern matching
        line.erase(std::remove_if(line.begin(), line.end(), ::isspace), line.end());

        if (in_cut_section) {
            // Check for section closing brace
            end_of_section = (line.find("};") != std::string::npos);
            if (end_of_section) continue;

            // Clean line and add to results
            line.erase(std::remove(line.begin(), line.end(), ','), line.end());
            cuts.push_back(line);
        } else {
            // Look for the start of our cut section
            in_cut_section = (line.find(cut_identifier) != std::string::npos);
        }
    }

    return cuts;
}

// Extract slice cuts from the configuration file
std::vector<std::string> SliceCuts() {
    return ExtractCuts("slice_cuts={");
}

// Extract spill cuts from the configuration file
std::vector<std::string> SpillCuts() {
    return ExtractCuts("spill_cuts={");
}
