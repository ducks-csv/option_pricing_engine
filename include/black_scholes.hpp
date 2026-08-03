#pragma once

enum class OptionType{Call, Put};

class BlackScholes{
    public:

    static double price(OptionType type, double S, double K, double T, double r, double sigma);

    static double cumulative_normal_distribution(double x);
};