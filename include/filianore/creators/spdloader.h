#ifndef _SPD_LOADER_H
#define _SPD_LOADER_H

#include "../color/cie.h"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using namespace filianore;

namespace filianore_spd_loader {

typedef struct
{
    std::vector<float> wavelengths;
    std::vector<float> values;
    size_t s_size;
} SpectralData;

bool LoadSPD(const char *filename, const char *SPDName, SpectralData &spectralData);
bool ParsedFile(const char *_filename, const char *SPDName, std::vector<std::string *> &_lines);
} // namespace filianore_spd_loader

#ifdef __SPD_LOADER_IMPLEMENTATION_H
#include "spdloader.h"

namespace filianore_spd_loader {

bool LoadSPD(const char *filename, const char *SPDName, SpectralData &spectralData) {
    std::vector<std::string *> file_lines;

    if (ParsedFile(filename, SPDName, file_lines)) {

        spectralData.wavelengths.reserve(file_lines.size());
        spectralData.values.reserve(file_lines.size());

        for (int i = 0; i < file_lines.size(); i++) {
            int temp1 = 0;
            float temp2 = 0.f;
            if (sscanf(file_lines[i]->c_str(), "%i %f", &temp1, &temp2) == 2) {
                spectralData.wavelengths.emplace_back((float)temp1);
                spectralData.values.emplace_back(temp2);
            }
        }

        spectralData.s_size = file_lines.size();

        std::cout << "Shader SPD parsed successfully." << std::endl;
        std::cout << "--------------------------------------------------------------------------------" << std::endl;
        return true;
    }
    // File not found or an error occured so return
    else {
        return false;
    }
}

bool ParsedFile(const char *_filename, const char *SPDName, std::vector<std::string *> &_lines) {

    std::ifstream file(_filename);
    std::string line;
    std::string lineIndic;

    if (file.is_open()) {
        std::cout << SPDName << " - SPD loaded successfully." << std::endl;

        std::cout << "Processing SPD...";
        while (!file.eof()) {
            getline(file, line);
            _lines.push_back(new std::string(line));
        }

        return true;
        file.close();
    } else {
        return false;
    }
}

} // namespace filianore_spd_loader

#endif

#endif
