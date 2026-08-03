#include <iostream>
#include "black_scholes.hpp"

int main(){
    double S = 100;
    double K = 100;
    double T = 1;
    double r = 0.05;
    double sigma = 0.2;

    double call_price = BlackScholes::price(OptionType::Call, S, K, T, r, sigma);
    double put_price = BlackScholes::price(OptionType::Put, S, K, T, r, sigma);

    std::cout << "Call Price: " << call_price << "\n";
    std::cout << "Put Price: " << put_price << "\n";
}