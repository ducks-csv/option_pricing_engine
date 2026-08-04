#include <pybind11/pybind11.h>
#include "black_scholes.hpp"
#include "monte_carlo.hpp"

namespace py = pybind11;

PYBIND11_MODULE(option_engine, m) {
    m.doc() = "High-Performance Option Pricing Engine in C++ with pybind11";

    py::enum_<OptionType>(m, "OptionType")
        .value("Call", OptionType::Call)
        .value("Put", OptionType::Put)
        .export_values();

    py::class_<Greeks>(m, "Greeks")
        .def_readwrite("delta", &Greeks::delta)
        .def_readwrite("gamma", &Greeks::gamma)
        .def_readwrite("vega", &Greeks::vega)
        .def("__repr__", [](const Greeks& g) {
            return "<Greeks delta=" + std::to_string(g.delta) + 
                   " gamma=" + std::to_string(g.gamma) + 
                   " vega=" + std::to_string(g.vega) + ">";
        });

    py::class_<BlackScholes>(m, "BlackScholes")
        .def_static("price", &BlackScholes::price, 
                    py::arg("type"), py::arg("S"), py::arg("K"), 
                    py::arg("T"), py::arg("r"), py::arg("sigma"),
                    "calculates the analytical black-scholes price.")
        .def_static("greeks", &BlackScholes::greeks,
                    py::arg("type"), py::arg("S"), py::arg("K"), 
                    py::arg("T"), py::arg("r"), py::arg("sigma"),
                    "calculates the analytical black-scholes greeks.");

    py::class_<MonteCarlo>(m, "MonteCarlo")
        .def_static("price", &MonteCarlo::price,
                    py::arg("type"), py::arg("S"), py::arg("K"), 
                    py::arg("T"), py::arg("r"), py::arg("sigma"),
                    py::arg("num_simulations"), py::arg("num_threads") = 0,
                    "calculates the option cost (premium) with multi-threaded Monte Carlo.")
        .def_static("greeks", &MonteCarlo::greeks,
                    py::arg("type"), py::arg("S"), py::arg("K"), 
                    py::arg("T"), py::arg("r"), py::arg("sigma"),
                    py::arg("num_simulations"), py::arg("h") = 0.01, 
                    py::arg("num_threads") = 0,
                    "calculates the greeks with monte carlo and finite difference method.");
}