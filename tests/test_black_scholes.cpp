#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include "black_scholes.hpp"
#include <cmath>

TEST_CASE("Black-Scholes Standard Price", "[black_scholes]"){
    double S = 100; // price
    double K = 100; // strike price
    double T = 1.0; // time (in years)
    double r = 0.05; // interest 
    double sigma = 0.2; // volatility

    SECTION("Call Option Pricing") {
        double call = BlackScholes::price(OptionType::Call, S, K, T, r, sigma);
        // margin of error musn't be larger than 0.1%, goal = $10.4506
        REQUIRE_THAT(call, Catch::Matchers::WithinRel(10.4506, 0.001));
    }

    SECTION("Put Option Pricing"){
        double put = BlackScholes::price(OptionType::Put, S, K, T, r, sigma);
        // margin of error musn't be larger than 0.1%, goal = $5.5735
        REQUIRE_THAT(put, Catch::Matchers::WithinRel(5.5735, 0.001));
    }
}

TEST_CASE("Put-Call-Parity", "[black_scholes]"){
    // C - P = S - K * exp(-r * T)
    double S = 120.0;
    double K = 100.0;
    double T = 0.5;
    double r = 0.03;
    double sigma = 0.25;

    double call = BlackScholes::price(OptionType::Call, S, K, T, r, sigma);
    double put = BlackScholes::price(OptionType::Put, S, K, T, r, sigma);

    double left_side = call - put;
    double right_side = S - K * std::exp(-r * T);
    // tolerance : 0.0001 (0.01%)
    REQUIRE_THAT(left_side, Catch::Matchers::WithinRel(right_side, 0.0001));
}