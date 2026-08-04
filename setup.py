from setuptools import setup
from pybind11.setup_helpers import Pybind11Extension, build_ext
import sys

__version__ = "0.0.1"

# Compiler-Flags 
extra_compile_args = []
if sys.platform == "win32":
    extra_compile_args = ["/std:c++17", "/O2"]
else:
    extra_compile_args = ["-std=c++17", "-O3"]

ext_modules = [
    Pybind11Extension(
        "option_engine",
        [
            "src/bindings.cpp",
            "src/black_scholes.cpp",
            "src/monte_carlo.cpp",
        ],
        include_dirs=["include"],
        extra_compile_args=extra_compile_args,
    ),
]

setup(
    name="option_engine",
    version=__version__,
    author="Alex McIntosh",
    description="High-Performance Option Pricing Engine C++ Bindings",
    ext_modules=ext_modules,
    cmdclass={"build_ext": build_ext},
    zip_safe=False,
    python_requires=">=3.8",
)