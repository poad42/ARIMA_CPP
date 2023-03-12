#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <stdexcept>

using namespace std;

// Define the class for each entry in the sunspot dataset
class SunspotEntry {
public:
    int year, month, day;
    double decimal_year, SNvalue, SNerror;
    int nb_observations;

    SunspotEntry(int y, int m, int d, double dy, double sn, double se, int n)
        : year(y), month(m), day(d), decimal_year(dy), SNvalue(sn), SNerror(se), nb_observations(n) {}
};

int main() {
    // Open the sunspot dataset file
    ifstream file("sunspots.csv");
    if (!file.is_open()) {
        throw runtime_error("Failed to open sunspots.csv");
    }

    // Declare variables for storing the data
    vector<SunspotEntry> data;
    string line, value;

    // Read the data from the file
    try {
        while (getline(file, line)) {
            stringstream ss(line);
            int y, m, d, n;
            double dy, sn, se;

            // Parse the values from the row
            if (getline(ss, value, ';')) y = stoi(value);
            if (getline(ss, value, ';')) m = stoi(value);
            if (getline(ss, value, ';')) d = stoi(value);
            if (getline(ss, value, ';')) dy = stod(value);
            if (getline(ss, value, ';')) sn = stod(value);
            if (getline(ss, value, ';')) se = stod(value);
            if (getline(ss, value, ';')) n = stoi(value);

            // Create a SunspotEntry object and add it to the vector
            SunspotEntry entry(y, m, d, dy, sn, se, n);
            data.push_back(entry);
        }
    }
    catch (const invalid_argument& ia) {
        throw runtime_error("Failed to convert data to double: " + string(ia.what()));
    }

    // Close the file
    file.close();

    // Print the first few entries as a table with columns
    cout << "Year\tMonth\tDay\tDecimal Year\tSN Value\tSN Error\tNb Observations" << endl;
    cout << "------------------------------------------------------------------" << endl;
    int num_entries_to_print = 20;  // Change this to print more or fewer entries
    for (int i = 0; i < num_entries_to_print && i < data.size(); i++) {
        cout << data[i].year << "\t" << data[i].month << "\t" << data[i].day << "\t"
             << data[i].decimal_year << "\t" << data[i].SNvalue << "\t" << data[i].SNerror << "\t"
             << data[i].nb_observations << endl;
    }

    return 0;
}