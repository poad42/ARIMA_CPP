#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <cmath>
#include "data_loader.h"

using namespace std;

std::vector<SunspotEntry> load_dataset(std::string filename) {
    std::vector<SunspotEntry> data;
    std::ifstream file(filename);
    std::string line;

    while (std::getline(file, line)) {
        std::stringstream ss(line);
        int y = 0, m = 0, d = 0, n = 0;
        double dy = 0.0, sn = 0.0, se = 0.0;
        std::string value;

        if (std::getline(ss, value, ';')) y = std::stoi(value);
        if (std::getline(ss, value, ';')) m = std::stoi(value);
        if (std::getline(ss, value, ';')) d = std::stoi(value);
        if (std::getline(ss, value, ';')) {
            if (value == "-nan") continue;
            dy = std::stod(value);
        }
        if (std::getline(ss, value, ';')) {
            if (value == "-nan") continue;
            sn = std::stod(value);
        }
        if (std::getline(ss, value, ';')) se = std::stod(value);
        if (std::getline(ss, value, ';')) n = std::stoi(value);

        SunspotEntry entry(y, m, d, dy, sn, se, n);
        data.push_back(entry);
    }

    return data;
}