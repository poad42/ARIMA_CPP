#include <vector>
#include <iostream>
#include <boost/math/distributions/students_t.hpp>
#include <boost/math/distributions.hpp>
#include "adf_test.h"

using namespace std;

bool adf_test(std::vector<SunspotEntry>& data, double& test_statistic, double& p_value) {
    std::vector<SunspotEntry>& data_copy = data;
    // Extract the SN values from the SunspotEntry vector
    std::vector<double> sn_values;
    for (const auto& entry : data_copy) {
        sn_values.push_back(entry.SNvalue);
    }

    // Remove any NaN values from the data_copy
    sn_values.erase(std::remove_if(sn_values.begin(), sn_values.end(),
                      [](double d) { return std::isnan(d); }), sn_values.end());

    // Calculate the mean and standard deviation of the data_copy
    double mean = 0, variance = 0;
    for (int i = 0; i < sn_values.size(); i++) {
        mean += sn_values[i];
        variance += sn_values[i] * sn_values[i];
    }
    mean /= sn_values.size();
    variance = variance / sn_values.size() - mean * mean;

    // Calculate the test statistic
    double x = 0, y = 0;
    for (int i = 1; i < sn_values.size(); i++) {
        x += (sn_values[i] - mean) * (sn_values[i-1] - mean);
        y += (sn_values[i-1] - mean) * (sn_values[i-1] - mean);
    }
    double rho = x / y;
    test_statistic = (rho - 1) * sqrt(sn_values.size() - 2) / sqrt(1 - rho*rho);

    // Calculate the p-value
    boost::math::students_t dist(sn_values.size() - 2);
    p_value = 2 * cdf(boost::math::complement(dist, abs(test_statistic)));

    // Determine whether the series is stationary or non-stationary
    double significance_level = 0.05;  // Change this to adjust the significance level
    bool is_stationary = p_value < significance_level;

    // Print the results of the test
    std::cout << "ADF test results:" << std::endl;
    std::cout << "Test statistic: " << test_statistic << std::endl;
    std::cout << "P-value: " << p_value << std::endl;
    if (is_stationary) {
        std::cout << "The series is stationary" << std::endl;
    } else {
        std::cout << "The series is non-stationary" << std::endl;
    }

    return is_stationary;
}