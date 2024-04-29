# fft_benchmarks
Benchmarks for Large Scale 2D FFTs


Results on large 2D FFTs from 32k * 32k upto 131k * 131k (keeping in line with the powers of 2)

Prerequisties:
- Benchmark : https://github.com/google/benchmark
- GCC
- FFTW3
- pocketfft : https://gitlab.mpcdf.mpg.de/mtr/pocketfft/-/tree/cpp?ref_type=heads
- OpenMP

Libraries tested:
- fftw
- oneapi:mkl
- pocketfft

build command for fftw benchmarks:
```
g++ -std=c++2a -o benchfftw benchmarking_fftw.cpp -fopenmp -lfftw3_threads -lfftw3_omp -lfftw3 -lm -lpthread -isystem benchmark/include -Lbenchmark/build/src -lbenchmark -lfftw3f -lfftw3f_threads

```

build command for pocketfft benchmarks:
NOTE: This command assumes you have pocketfft downloaded at an appropriate path

```
g++ -std=c++2a -o benchpocket benchmarking_pocket.cpp -I ../pocketfft/pocketfft_hdronly.h -isystem benchmark/include -Lbenchmark/build/src -lbenchmark -lpthread

```

build command for MKL benchmarks:
NOTE: This command assumes you have MKL library installed in the deafult opt location. 

```
g++ -o benchmkl benchmarking_mkl.cpp -I "/opt/intel/oneapi/mkl/2024.1/include" -m64 -Wl,--start-group /opt/intel/oneapi/mkl/2024.1/lib/libmkl_intel_ilp64.a /opt/intel/oneapi/mkl/2024.1/lib/libmkl_gnu_thread.a /opt/intel/oneapi/mkl/2024.1/lib/libmkl_core.a -Wl,--end-group -lgomp -lpthread -lm -ldl -isystem benchmark/include -Lbenchmark/build/src -lbenchmark

```
