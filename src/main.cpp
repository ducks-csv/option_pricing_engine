#include <iostream>
#include <iomanip>
#include <chrono>
#include "black_scholes.hpp"
#include "monte_carlo.hpp"

int main() {

    // market option parameters
    double S = 100.0;          // Current stock price (spot price)
    double K = 100.0;          // Strike price
    double T = 1.0;            // Time to maturity in years (1 year)
    double r = 0.05;           // Risk-free rate (5%)
    double sigma = 0.20;       // Implied volatility (20%)
    std::size_t num_sims = 2000000; // 2 million Monte Carlo paths

    unsigned int num_threads = 0;

    std::cout << "====================================================\n";
    std::cout << "    HIGH-PERFORMANCE OPTION PRICING ENGINE (C++)    \n";
    std::cout << "====================================================\n\n";

    std::cout << "Parameters:\n";
    std::cout << "  Spot Price (S):    " << S << "\n";
    std::cout << "  Strike Price (K):  " << K << "\n";
    std::cout << "  Time to Maturity (T):  " << T << " years\n";
    std::cout << "  Risk-free Rate (r):" << r * 100 << "%\n";
    std::cout << "  Volatility (sigma):" << sigma * 100 << "%\n";
    std::cout << "  MC Simulations:    " << num_sims << "\n\n";

    // floating-point 4 decimal places
    std::cout << std::fixed << std::setprecision(4);

    std::cout << "--- 1. Analytical Black-Scholes Model ---\n";
    
    double bs_call_price = BlackScholes::price(OptionType::Call, S, K, T, r, sigma);
    double bs_put_price = BlackScholes::price(OptionType::Put, S, K, T, r, sigma);
    Greeks bs_greeks = BlackScholes::greeks(OptionType::Call, S, K, T, r, sigma);

    std::cout << "  Call Price: " << bs_call_price << "\n";
    std::cout << "  Put Price:  " << bs_put_price << "\n";
    std::cout << "  Greeks (Call):\n";
    std::cout << "    Delta (dC/dS):  " << bs_greeks.delta << "\n";
    std::cout << "    Gamma (d2C/dS2):" << bs_greeks.gamma << "\n";
    std::cout << "    Vega  (dC/dVol):" << bs_greeks.vega << "\n\n";

    std::cout << "--- 2. Multi-Threaded Monte Carlo Simulation ---\n";

    // Start timing the price simulation
    auto start_time = std::chrono::high_resolution_clock::now();

    double mc_call_price = MonteCarlo::price(OptionType::Call, S, K, T, r, sigma, num_sims, num_threads);
    
    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> duration = end_time - start_time;

    Greeks mc_greeks = MonteCarlo::greeks(OptionType::Call, S, K, T, r, sigma, num_sims, 0.01, num_threads);

    std::cout << "  Call Price: " << mc_call_price << "\n";
    std::cout << "  Computation Time: " << duration.count() << " ms\n";
    std::cout << "  Greeks (Call - Finite Difference):\n";
    std::cout << "    Delta (dC/dS):  " << mc_greeks.delta << "\n";
    std::cout << "    Gamma (d2C/dS2):" << mc_greeks.gamma << "\n\n";


    std::cout << "--- 3. Result Comparison (Call Option) ---\n";
    double price_diff = std::abs(bs_call_price - mc_call_price);
    double delta_diff = std::abs(bs_greeks.delta - mc_greeks.delta);
    double gamma_diff = std::abs(bs_greeks.gamma - mc_greeks.gamma);

    std::cout << "  Price Difference (Absolute): " << price_diff << "\n";
    std::cout << "  Delta Difference (Absolute): " << delta_diff << "\n";
    std::cout << "  Gamma Difference (Absolute): " << gamma_diff << "\n";
    std::cout << "====================================================\n";

    return 0;
}