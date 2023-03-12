#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <cmath>
#include "data_loader.h"

using namespace std;

vector<SunspotEntry> load_dataset(string filename) {
    vector<SunspotEntry> data;
    ifstream file(filename);
    string line;

    // Remove the unused variable `line`
    while (getline(file, line)) {
        stringstream ss(line);
        int y = 0, m = 0, d = 0, n = 0;
        double dy = 0.0, sn = 0.0, se = 0.0;
        string value;

        if (getline(ss, value, ';')) y = stoi(value);
        if (getline(ss, value, ';')) m = stoi(value);
        if (getline(ss, value, ';')) d = stoi(value);
        if (getline(ss, value, ';')) {
            if (value == "-nan") continue;  // Skip this row
            dy = stod(value);
        }
        if (getline(ss, value, ';')) {
            if (value == "-nan") continue;  // Skip this row
            sn = stod(value);
        }
        if (getline(ss, value, ';')) se = stod(value);
        if (getline(ss, value, ';')) n = stoi(value);

        SunspotEntry entry(y, m, d, dy, sn, se, n);
        data.push_back(entry);
    }

    return data;
}