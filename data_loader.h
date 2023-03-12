#ifndef DATA_LOADER_H
#define DATA_LOADER_H

#include <string>
#include <vector>

// Define the class for each entry in the sunspot dataset
class SunspotEntry {
public:
    int year, month, day;
    double decimal_year, SNvalue, SNerror;
    int nb_observations;

    SunspotEntry(int y, int m, int d, double dy, double sn, double se, int n)
        : year(y), month(m), day(d), decimal_year(dy), SNvalue(sn), SNerror(se), nb_observations(n) {}
};

// Function to load the sunspot dataset from a file
std::vector<SunspotEntry> load_dataset(std::string filename);

#endif