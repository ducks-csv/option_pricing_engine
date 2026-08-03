#include "black_scholes.hpp"
#include <cmath>

double BlackScholes::cumulative_normal_distribution(double x){

    return 0.5 * std::erfc(-x * M_SQRT1_2);
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