#include <vector>
#include <iostream>
#include <boost/math/distributions/students_t.hpp>
#include <boost/math/distributions.hpp>
#include "adf_test.h"

using namespace std;

bool adf_test(vector<double>& data, double& test_statistic, double& p_value) {
    // Remove any NaN values from the data
    data.erase(remove_if(data.begin(), data.end(), [](double d) { return isnan(d); }), data.end());

    // Calculate the mean and standard deviation of the data
    double mean = 0, variance = 0;
    for (int i = 0; i < data.size(); i++) {
        mean += data[i];
        variance += data[i] * data[i];
    }
    mean /= data.size();
    variance = variance / data.size() - mean * mean;

    // Calculate the test statistic
    double x = 0, y = 0;
    for (int i = 1; i < data.size(); i++) {
        x += (data[i] - mean) * (data[i-1] - mean);
        y += (data[i-1] - mean) * (data[i-1] - mean);
    }
    double rho = x / y;
    test_statistic = (rho - 1) * sqrt(data.size() - 2) / sqrt(1 - rho*rho);
    
    // Calculate the p-value
    boost::math::students_t dist(data.size() - 2);
    p_value = 2 * cdf(boost::math::complement(dist, abs(test_statistic)));

    // Determine whether the series is stationary or non-stationary
    double significance_level = 0.05;  // Change this to adjust the significance level
    bool is_stationary = p_value < significance_level;
    
    // Print the results of the test
    cout << "ADF test results:" << endl;
    cout << "Test statistic: " << test_statistic << endl;
    cout << "P-value: " << p_value << endl;
    if (is_stationary) {
        cout << "The series is stationary" << endl;
    } else {
        cout << "The series is non-stationary" << endl;
    }

    return is_stationary;
}