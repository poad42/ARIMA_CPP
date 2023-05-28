#include <iostream>
#include <cmath>
#include <stdexcept>
#include <boost/math/special_functions/gamma.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/lu.hpp>
#include "arima_modeling.h"

using namespace std;


arma::vec acf(const std::vector<SunspotEntry>& data, int maxlag) {
    int n = data.size();
    arma::vec x(n);
    for (int i = 0; i < n; i++) {
        x(i) = data[i].SNvalue;
    }

    arma::vec acf(maxlag+1);
    for (int k = 0; k <= maxlag; k++) {
        double sum = 0;
        for (int i = k; i < n; i++) {
            sum += x(i) * x(i-k);
        }
        acf(k) = sum / (n - k);
    }
    return acf;
}

arma::vec pacf(const std::vector<SunspotEntry>& data, int maxlag) {
    int n = data.size();
    arma::vec x(n);
    for (int i = 0; i < n; i++) {
        x(i) = data[i].SNvalue;
    }

    arma::vec pacf_vals(maxlag+1, arma::fill::zeros);
    arma::mat R(maxlag+1, maxlag+1, arma::fill::zeros);

    // Compute autocorrelations for different lags
    for (int i = 0; i <= maxlag; i++) {
        for (int j = i; j < n; j++) {
            R(i, 0) += x[j] * x[j - i];
        }
        R(i, 0) /= (n - i);
    }

    // Normalize the first value to 1
    pacf_vals(0) = 1.0;
    R(0, 0) = 1.0;

    // Apply the Durbin-Levinson algorithm to solve Yule-Walker equations
    for (int k = 1; k <= maxlag; k++) {
        double num = R(k, 0);
        for (int j = 1; j < k; j++) {
            num -= R(k - j, 0) * R(j, k - 1);
        }
        double den = 1.0;
        for (int j = 1; j < k; j++) {
            den -= R(j, 0) * R(j, k - 1);
        }
        pacf_vals(k) = num / den;

        for (int j = 1; j < k; j++) {
            R(j, k) = R(j, k - 1) - pacf_vals(k) * R(k - j, k - 1);
        }
        R(k, k) = pacf_vals(k);
    }

    return pacf_vals;
}

std::pair<int, int> get_pq(const std::vector<SunspotEntry>& data) {
    int max_order = 5;
    int n = data.size();
    cout << "n = " << n << endl;
    arma::vec acf_vals = acf(data, max_order);
    arma::vec pacf_vals = pacf(data, max_order);
    double threshold = 1.96 / sqrt(n);

    for (int p = 0; p < max_order; p++) {
        if (std::abs(pacf_vals(p+1)) < threshold) {
            for (int q = 0; q < max_order; q++) {
                if (std::abs(acf_vals(q+1)) < threshold) {
                    return std::make_pair(p, q);
                }
            }
        }
    }

    return std::make_pair(0, 0);
}
// Fit an ARMA model to a time series using maximum likelihood estimation
ARIMAModel fit_arma_model(const vector<SunspotEntry>& data, int p, int q) {
    int n = data.size();
    arma::vec data_vec(n);
    for (int i = 0; i < n; i++) {
        data_vec(i) = data[i].SNvalue;
    }
    arma::vec residuals(n + std::max(p, q));

    // Fit the ARMA model using maximum likelihood estimation
    arma::vec phi(p), theta(q);
    arma::mat X(n-q, p+q);
    for (int i = q; i < n; i++) {
        residuals(i) = data_vec(i);
        for (int j = 0; j < p; j++) {
            X(i-q, j) = data_vec(i-j-1);
        }
        for (int j = 0; j < q; j++) {
            X(i-q, p+j) = residuals(i-j-1);
        }
    }
    arma::vec coefficients = arma::solve(X, residuals);

    for (int i = 0; i < p; i++) {
        phi(i) = coefficients(i);
    }
    for (int i = 0; i < q; i++) {
        theta(i) = coefficients(p+i);
    }

    // Compute the residuals of the ARMA model
    for (int i = p+q; i < n; i++) {
        double y_pred = 0;
        for (int j = 0; j < p; j++) {
            y_pred += phi(j) * residuals(i-j-1);
        }
        for (int j = 0; j < q; j++) {
            y_pred += theta(j) * residuals(i-j-1);
        }
        residuals(i) = data_vec(i) - y_pred;
    }

    // Compute the log likelihood of the ARMA model parameters
    double log_likelihood = -n * log(sqrt(2 * M_PI)) - 0.5 * arma::dot(residuals, residuals) / n;

    // Calculate the Akaike Information Criterion (AIC)
    int num_params = p + q + 1;
    double aic = -2 * log_likelihood + 2 * num_params;

    // Calculate the Bayesian Information Criterion (BIC)
    int num_observations = data_vec.n_elem;
    double bic = -2 * log_likelihood + num_params * log(num_observations);

    // Print the model parameters and evaluation metrics
    cout << "ARMA(" << p << ", " << q << ") model:" << endl;
    cout << "AR coefficients: " << phi.t() << endl;
    cout << "MA coefficients: " << theta.t() << endl;
    cout << "Residuals variance: " << arma::var(residuals) << endl;
    cout << "Log likelihood: " << log_likelihood << endl;
    cout << "AIC: " << aic << endl;
    cout << "BIC: " << bic << endl;

    // Store the model parameters and evaluation metrics
    ARIMAModel model;
    model.p = p;
    model.q = q;
    model.phi = phi;
    model.theta = theta;
    model.residuals_var = arma::var(residuals);
    model.log_likelihood = log_likelihood;
    model.aic = aic;
    model.bic = bic;

    return model;
}

double evaluate(const std::vector<SunspotEntry>& data, ARIMAModel model) {
    int n = data.size();
    arma::vec phi(model.p), theta(model.q);
    double rmse;
    arma::vec predictions;
    predictions.resize(n);

    for (int i = model.p + model.q; i < n; i++) {
        double y_pred = 0;
        for (int j = 0; j < model.p; j++) {
            y_pred += phi(j) * data[i - j - 1].SNvalue;
        }
        for (int j = 0; j < model.q; j++) {
            y_pred += theta(j) * predictions[i - j - 1];
        }
        predictions(i) = y_pred;
    }

    arma::vec data_values(n);
    for (int i = 0; i < n; i++) {
        data_values(i) = data[i].SNvalue;
    }
    
    rmse = sqrt(arma::mean(arma::pow(predictions - data_values, 2)));

    return rmse;
}