#pragma once

enum class OptionType{Call, Put};

struct Greeks{
    double delta;
    double gamma;
    double vega;
};

class BlackScholes{
    public:

    static double price(OptionType type, double S, double K, double T, double r, double sigma);

    static Greeks greeks(OptionType type, double S, double K, double T, double r, double sigma);

    private:
    static double cumulative_normal_distribution(double x);
    static double normal_pdf(double x);
};