#include <cmath>
#include <random>
#include <vector>
#include <thread>
#include <numeric>
#include "monte_carlo.hpp"

double MonteCarlo::price(OptionType type, double S, double K, double T, double r, double sigma,
    std::size_t num_simulations, unsigned int num_threads) {

        if (num_threads == 0) {
            num_threads = std::thread::hardware_concurrency();
            if (num_threads == 0) num_threads = 4;
        }
        std::size_t sims_per_thread = num_simulations / num_threads;
        std::vector<double> partial_sums(num_threads, 0.0);
        std::vector<std::thread> threads;

        // geometric brownian motion

        double drift = (r - 0.5 * sigma * sigma) * T;
        double vol_sqrt_T = sigma * std::sqrt(T);
        double discount_factor = std::exp(-r * T);

        for (unsigned int i = 0; i < num_threads; ++i){
            std::size_t count = (i == num_threads - 1) ? (num_simulations - i * sims_per_thread) : sims_per_thread;

            threads.emplace_back([i, count, S, K, type, drift, vol_sqrt_T, &partial_sums](){
                std::mt19937_64 rng(1739 + i);
                std::normal_distribution<double> norm_dist(0.0, 1.0);

                double thread_payoff_sum = 0.0;

                for (std::size_t sim = 0; sim < count; ++sim){
                    double Z = norm_dist(rng);
                    double ST = S * std::exp(drift + vol_sqrt_T * Z);

                    double payoff = 0.0;
                    if (type == OptionType::Call){
                        payoff = std::max(0.0, ST - K);
                    } else {
                        payoff = std::max(0.0, K - ST);
                    }
                    thread_payoff_sum += payoff;
                }
                partial_sums[i] = thread_payoff_sum;
            });
            
        }

        for (auto& t : threads) {
            if (t.joinable()){
                t.join();
            }
        }

        double total_payoff_sum = std::accumulate(partial_sums.begin(), partial_sums.end(), 0.0);
        return discount_factor * (total_payoff_sum / static_cast<double>(num_simulations));
}

Greeks MonteCarlo::greeks(OptionType type, double S, double K, double T, double r, double sigma,
                         std::size_t num_simulations, double h, unsigned int num_threads){

    
    double price_up   = MonteCarlo::price(type, S + h, K, T, r, sigma, num_simulations, num_threads);
    double price_base = MonteCarlo::price(type, S,K, T, r, sigma, num_simulations, num_threads);
    double price_down = MonteCarlo::price(type, S - h, K, T, r, sigma, num_simulations, num_threads);

    Greeks g;

    g.delta = (price_up - price_down) / (2.0 * h);
    g.gamma = (price_up - 2.0 * price_base + price_down) / (h*h);
    g.vega = 0.0;

    return g;
}