# Sunspot Data Analysis

This program performs time series analysis on the monthly mean total sunspot number (SN) data from January 1749 to December 2019. The program includes functions for visualizing the data, fitting an ARIMA model to the data, and evaluating the model.

## Data
The data is stored in the file "sunspot.csv" and contains the following columns:
- Year: the year of the observation
- Month: the month of the observation
- Date: the date of the observation in YYYY-MM-DD format
- SNvalue: the monthly mean total sunspot number

## Visualization
The program includes a function to visualize the data using a line plot. The plot shows the monthly mean total sunspot number as a function of time.

## ARIMA Modeling
The program includes a function to fit an ARIMA model to the data. The function uses the auto.arima algorithm to select the optimal model order based on the AIC criterion. The function returns the fitted model, which includes the following parameters:
- p: the AR order of the model
- d: the differencing order of the model
- q: the MA order of the model
- phi: the AR coefficients
- theta: the MA coefficients
- phi_stderr: the standard errors of the AR coefficients
- theta_stderr: the standard errors of the MA coefficients
- residuals_var: the variance of the residuals
- log_likelihood: the log likelihood of the model parameters
- aic: the Akaike Information Criterion (AIC) of the model
- bic: the Bayesian Information Criterion (BIC) of the model

## Model Evaluation
The program includes a function to evaluate the fitted ARIMA model using the RMSE (root-mean-square error) metric. The function returns the RMSE value of the fitted model.

## ADF Test
The program includes a function to perform the Augmented Dickey-Fuller (ADF) test on the data. The function tests the null hypothesis that the data has a unit root and is non-stationary. The function returns the test statistic and p-value of the test, as well as a boolean value indicating whether the data is stationary or non-stationary.

## Usage
To run the program, run the following command from the terminal:

$ make

This will compile the program and create an executable file named "main". To run the program, execute the following command:

$ ./main

This will run the program and output the results of the analysis.

## Dependencies
- C++11 or later
- Armadillo library (http://arma.sourceforge.net/)
- Boost library (https://www.boost.org/)
- GNU Make (https://www.gnu.org/software/make/)

## Author
- Name: Adhitya Mohan
- Email: me@adhityamohan.in