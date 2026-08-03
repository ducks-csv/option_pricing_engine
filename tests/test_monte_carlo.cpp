#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include "black_scholes.hpp"
#include "monte_carlo.hpp"

TEST_CASE("Monte Carlo vs. Analytical Black-Scholes", "[monte_carlo]") {
    double S = 100.0;
    double K = 100.0;
    double T = 1.0;
    double r = 0.05;
    double sigma = 0.2;
    std::size_t num_sims = 1000000; 

    double analytical_call = BlackScholes::price(OptionType::Call, S, K, T, r, sigma);
    double mc_call = MonteCarlo::price(OptionType::Call, S, K, T, r, sigma, num_sims);

    SECTION("Call Price Convergence") {
        // 
        // at 1_000_000 simulation we expect a rel. differnce of < 0.5%
        REQUIRE_THAT(mc_call, Catch::Matchers::WithinRel(analytical_call, 0.005));
    }
}

TEST_CASE("Monte Carlo Greeks vs. Analytical Greeks", "[monte_carlo]") {
    double S = 100.0;
    double K = 100.0;
    double T = 1.0;
    double r = 0.05;
    double sigma = 0.2;
    std::size_t num_sims = 2000000;

    Greeks bs_greeks = BlackScholes::greeks(OptionType::Call, S, K, T, r, sigma);
    Greeks mc_greeks = MonteCarlo::greeks(OptionType::Call, S, K, T, r, sigma, num_sims, 0.01);

    SECTION("Delta Convergence") {
        // delta should be within 1% of the analytically derived value of delta
        REQUIRE_THAT(mc_greeks.delta, Catch::Matchers::WithinRel(bs_greeks.delta, 0.01));
    }
}