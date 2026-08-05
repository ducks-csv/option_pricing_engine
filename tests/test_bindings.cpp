#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include <pybind11/embed.h>

namespace py = pybind11;

TEST_CASE("Python module binding import and API", "[bindings]") {
    py::scoped_interpreter guard{};
    py::module_ sys = py::module_::import("sys");
    sys.attr("path").attr("insert")(0, ".");

    py::module_ option_engine = py::module_::import("option_engine");
    py::object OptionType = option_engine.attr("OptionType");
    py::object call_type = OptionType.attr("Call");
    py::object put_type = OptionType.attr("Put");

    double S = 100.0;
    double K = 100.0;
    double T = 1.0;
    double r = 0.05;
    double sigma = 0.2;

    SECTION("OptionType values are exposed") {
        REQUIRE(call_type);
        REQUIRE(put_type);
        REQUIRE_FALSE(call_type.is(put_type));
    }

    SECTION("BlackScholes.price and greeks are callable") {
        double call_price = option_engine.attr("BlackScholes").attr("price")(call_type, S, K, T, r, sigma).cast<double>();
        REQUIRE_THAT(call_price, Catch::Matchers::WithinRel(10.4506, 0.001));

        py::object greeks = option_engine.attr("BlackScholes").attr("greeks")(call_type, S, K, T, r, sigma);
        REQUIRE_THAT(greeks.attr("delta").cast<double>(), Catch::Matchers::WithinRel(0.6368, 0.001));
        REQUIRE(greeks.attr("gamma").cast<double>() > 0.0);
        REQUIRE(greeks.attr("vega").cast<double>() > 0.0);
    }

    SECTION("MonteCarlo.price approximates analytical price") {
        double analytical_price = option_engine.attr("BlackScholes").attr("price")(call_type, S, K, T, r, sigma).cast<double>();
        double monte_carlo_price = option_engine.attr("MonteCarlo").attr("price")(call_type, S, K, T, r, sigma, 200000).cast<double>();
        REQUIRE_THAT(monte_carlo_price, Catch::Matchers::WithinRel(analytical_price, 0.03));
    }
}
