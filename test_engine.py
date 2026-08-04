import option_engine as oe

# analytical Black-Scholes Call
bs_call = oe.BlackScholes.price(oe.OptionType.Call, 100.0, 100.0, 1.0, 0.05, 0.2)
print(f"analytical call price via python: {bs_call:.4f}")

# multi-threaded monte carlo Call 1m. simulations
mc_call = oe.MonteCarlo.price(oe.OptionType.Call, 100.0, 100.0, 1.0, 0.05, 0.2, 1000000)
print(f"monte carlo call price via python:  {mc_call:.4f}")

# greeks
bs_greeks = oe.BlackScholes.greeks(oe.OptionType.Call, 100.0, 100.0, 1.0, 0.05, 0.2)
print(f"call Delta black-scholes: {bs_greeks.delta:.4f}, Gamma: {bs_greeks.gamma:.4f}")

mc_greeks = oe.MonteCarlo.greeks(oe.OptionType.Call, 100.0, 100.0, 1.0, 0.05, 0.2, 1000000)
print(f"call Delta monte carlo: {mc_greeks.delta:.4f}, Gamma: {mc_greeks.gamma:.4f}")