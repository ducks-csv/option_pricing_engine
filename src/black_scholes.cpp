#include "black_scholes.hpp"
#include <cmath>
#include <numbers>

double BlackScholes::cumulative_normal_distribution(double x){

    return 0.5 * std::erfc(-x * M_SQRT1_2);
}

double BlackScholes::normal_pdf(double x){
    double const inv_sqrt_2pi = std::numbers::inv_sqrtpi / std::numbers::sqrt2;
    return inv_sqrt_2pi * std::exp(-0.5 * x * x);
}

double BlackScholes::price(OptionType type, double S, double K, double T, double r, double sigma){
    double d1 = (std::log(S / K) + (r + 0.5 * sigma * sigma) * T) / (sigma * std::sqrt(T));
    double d2 = d1 - sigma * std::sqrt(T);

    if (type == OptionType::Call){
        return (S * cumulative_normal_distribution(d1) - K * std::exp(-r * T) * cumulative_normal_distribution(d2));
    }
    else {
        return K * std::exp(-r * T) * cumulative_normal_distribution(-d2)- S * cumulative_normal_distribution(-d1);
    }
}

Greeks BlackScholes::greeks(OptionType type, double S, double K, double T, double r, double sigma){
    
    double sqrt_T = std::sqrt(T);
    double d1 = (std::log(S / K) + (r + 0.5 * sigma * sigma) * T) / (sigma * sqrt_T);
    double normal_pdf_d1 = BlackScholes::normal_pdf(d1);
    Greeks g;

    g.gamma = normal_pdf_d1 / S * r * sqrt_T;
    g.vega = S * sqrt_T * normal_pdf_d1;

    if (type == OptionType::Call){
        g.delta = cumulative_normal_distribution(d1);
    } else {
        g.delta = cumulative_normal_distribution(d1) - 1.0;
    } 
    return g;
}