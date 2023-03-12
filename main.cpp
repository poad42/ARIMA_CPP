#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>
#include "data_loader.h"
#include "adf_test.h"
#include "arima_modeling.h"

using namespace std;

int main() {
    // Load the sunspot dataset
    string filename = "sunspots.csv";
    std::vector<SunspotEntry> data;
    try {
        data = load_dataset(filename);
    }
    catch (const runtime_error& e) {
        cerr << "Error loading data from " << filename << ": " << e.what() << endl;
        return 1;
    }

    // Perform the ADF test
    double test_statistic, p_value;
    bool is_stationary = adf_test(data, test_statistic, p_value);

    // get p and q values
    std::pair<int, int> pq = get_pq(data);

    // Print the first few entries of the dataset
    cout << "First few entries of " << filename << ":" << endl;
    cout << "Year\tMonth\tDay\tSNvalue" << endl;
    for (size_t i = 0; i < 10 && i < data.size(); i++) {
        cout << data[i].year << "\t" << data[i].month << "\t" << data[i].day << "\t" << data[i].SNvalue << endl;
    }

    if (is_stationary) {
        // Fit an ARIMA model to the data
        ARIMAModel arima_model;
        int p = pq.first;
        int q = pq.second;
        arima_model = fit_arma_model(data, p, q);

        // Print the ARIMA model summary
        cout << "ARIMA model summary:" << endl;
        cout << "p = " << arima_model.p << endl;
        cout << "d = " << arima_model.d << endl;
        cout << "q = " << arima_model.q << endl;
        cout << "phi = " << arima_model.phi.t() << endl;
        cout << "theta = " << arima_model.theta.t() << endl;
        cout << "Residuals variance = " << arima_model.residuals_var << endl;
        cout << "log likelihood = " << arima_model.log_likelihood << endl;
        cout << "AIC = " << arima_model.aic << endl;
        cout << "BIC = " << arima_model.bic << endl;

        // Evaluate the ARIMA model
        double rmse;
        rmse = evaluate(data, arima_model);
        cout << "ARIMA model RMSE: " << rmse << endl;
    } else {
        cout << "ARIMA modeling requires stationary time series data." << endl;
    }

    return 0;
}