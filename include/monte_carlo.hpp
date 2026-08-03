#pragma once

#include <cstddef>
#include "black_scholes.hpp"

class MonteCarlo{
    public:
    static double price(OptionType type, double S, double K, double T, double r, double sigma,
                        std::size_t num_simulations, unsigned int num_threads = 0);
    static Greeks greeks(OptionType type, double S, double K, double T, double r, double sigma,
                        std::size_t num_simulations, double h = 0.01, unsigned int num_threads = 0);

};