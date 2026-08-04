import time
import math
import random
import matplotlib.pyplot as plt
import option_engine as oe

# python implementation of monte carlo simulation
def monte_carlo_python(option_type, S, K, T, r, sigma, num_simulations):
    """
    simulates the option price (premium) using python.
    functions as a baseline vs the c++ engine.
    """
    drift = (r - 0.5 * sigma * sigma) * T
    vol_sqrt_T = sigma * math.sqrt(T)
    discount_factor = math.exp(-r * T)
    
    payoff_sum = 0.0
    
    
    for i in range(num_simulations):
        Z = random.gauss(0.0, 1.0)
        ST = S * math.exp(drift + vol_sqrt_T * Z)
        
        if option_type == "Call":
            payoff = max(0.0, ST - K)
        else:
            payoff = max(0.0, K - ST)
            
        payoff_sum += payoff
        
    return discount_factor * (payoff_sum / num_simulations)

# benchmark
def run_benchmark():
    # test parameters
    S, K, T, r, sigma = 100.0, 100.0, 1.0, 0.05, 0.2
    
    # sample size
    sim_counts = [100_000, 500_000, 1_000_000, 2_000_000, 10_000_000, 50_000_000]
    
    py_times = []
    cpp_single_times = []
    cpp_multi_times = []
    
    print("==========================================================")
    print("      STARTING BENCHMARK: PURE PYTHON VS C++ ENGINE       ")
    print("==========================================================\n")
    
    for N in sim_counts:
        print(f"--- simulation count: {N:,} ---")
        
        # python calculation
        start = time.perf_counter()
        tmp = monte_carlo_python("Call", S, K, T, r, sigma, N)
        dt_py = (time.perf_counter() - start) * 1000.0 # in ms
        py_times.append(dt_py)
        print(f"  Pure Python:      {dt_py:8.2f} ms")

        # C++ single-threaded
        start = time.perf_counter()
        tmp = oe.MonteCarlo.price(oe.OptionType.Call, S, K, T, r, sigma, N, 1)
        dt_cpp_single = (time.perf_counter() - start) * 1000.0 # in ms
        cpp_single_times.append(dt_cpp_single)
        print(f"  C++ (1 Thread):   {dt_cpp_single:8.2f} ms")
        
        # C++ Multi threaded (all threads)
        start = time.perf_counter()
        _ = oe.MonteCarlo.price(oe.OptionType.Call, S, K, T, r, sigma, N, 0)
        dt_cpp_multi = (time.perf_counter() - start) * 1000.0 # in ms
        cpp_multi_times.append(dt_cpp_multi)
        print(f"  C++ (all threads):{dt_cpp_multi:8.2f} ms\n")

    # plot
    
    plt.style.use('seaborn-v0_8-darkgrid' if 'seaborn-v0_8-darkgrid' in plt.style.available else 'default')
    fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(14, 6))
    
  
    sim_labels = [f"{N//1000}k" if N < 1_000_000 else f"{N//1_000_000}M" for N in sim_counts]
    
    valid_py_counts = [sim_labels[i] for i, t in enumerate(py_times) if t is not None]
    valid_py_times = [t for t in py_times if t is not None]
    
    ax1.plot(valid_py_counts, valid_py_times, 'o-', color='red', label='Python', linewidth=2)
    ax1.plot(sim_labels, cpp_single_times, 's-', color='blue', label='C++ (1 Thread)', linewidth=2)
    ax1.plot(sim_labels, cpp_multi_times, '^--', color='green', label='C++ (Multi-Threaded)', linewidth=2)
    
    ax1.set_title("runtime comparison (runtime in ms)", fontsize=12, fontweight='bold')
    ax1.set_xlabel("simulation count", fontsize=10)
    ax1.set_ylabel("time (ms)", fontsize=10)
    ax1.legend()
    ax1.grid(True)
    
    idx_1m = sim_counts.index(1_000_000)
    base_time = py_times[idx_1m]
    
    speedup_cpp_single = base_time / cpp_single_times[idx_1m]
    speedup_cpp_multi = base_time / cpp_multi_times[idx_1m]
    
    categories = ['Pure Python\n(Baseline)', 'C++ Engine\n(1 Thread)', 'C++ Engine\n(Multi-Threaded)']
    speedups = [1.0, speedup_cpp_single, speedup_cpp_multi]
    colors = ['gray', 'blue', 'green']
    
    bars = ax2.bar(categories, speedups, color=colors, width=0.5)
    ax2.set_title("Speedup vs. Pure Python (at 1 million simulations)", fontsize=12, fontweight='bold')
    ax2.set_ylabel("speed-up factor", fontsize=10)
    
    for bar in bars:
        height = bar.get_height()
        ax2.annotate(f'{height:.1f}x',
                    xy=(bar.get_x() + bar.get_width() / 2, height),
                    xytext=(0, 3),  # 3 points vertical offset
                    textcoords="offset points",
                    ha='center', va='bottom', fontweight='bold')

    plt.tight_layout()
    plt.savefig("benchmark_results.png", dpi=300)
    print("benchmark successfully finished. Diagramm saved as 'benchmark_results.png")
    plt.show()

if __name__ == "__main__":
    run_benchmark()