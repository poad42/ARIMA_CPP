#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>
#include "data_loader.h"
#include "adf_test.h"

using namespace std;

int main() {
    // Load the sunspot dataset
    string filename = "sunspots.csv";
    vector<SunspotEntry> data;
    try {
        data = load_dataset(filename);
    }
    catch (const runtime_error& e) {
        cerr << "Error loading data from " << filename << ": " << e.what() << endl;
        return 1;
    }

    // Convert the data to a vector of double values
    vector<double> values;
    for (int i = 0; i < data.size(); i++) {
        values.push_back(data[i].SNvalue);
    }

    // Print the first few entries of the dataset
    cout << "First few entries of " << filename << ":" << endl;
    cout << "Year\tMonth\tDay\tSNvalue" << endl;
    for (int i = 0; i < 10 && i < data.size(); i++) {
        cout << data[i].year << "\t" << data[i].month << "\t" << data[i].day << "\t" << data[i].SNvalue << endl;
    }

    // Perform the ADF test
    double test_statistic, p_value;
    bool is_stationary = adf_test(values, test_statistic, p_value);

    return 0;
}