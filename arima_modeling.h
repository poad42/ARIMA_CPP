#ifndef ARIMA_MODELING_H
#define ARIMA_MODELING_H

#include <vector>
#include <armadillo>
#include "data_loader.h"

struct ARIMAModel {
    int p, d, q;                    // ARIMA model orders
    arma::vec phi, theta;           // AR and MA coefficients
    arma::vec phi_stderr, theta_stderr; // Standard errors of AR and MA coefficients
    double residuals_var;           // Residuals variance
    double log_likelihood;          // Log likelihood of the model parameters
    double aic;                     // Akaike Information Criterion (AIC)
    double bic;                     // Bayesian Information Criterion (BIC)
};

ARIMAModel fit_arma_model(const std::vector<SunspotEntry>& data, int p, int q);
double evaluate(const std::vector<SunspotEntry>& data, ARIMAModel model);
std::pair<int, int> get_pq(const std::vector<SunspotEntry>& data);

#endif  // ARIMA_MODELING_H