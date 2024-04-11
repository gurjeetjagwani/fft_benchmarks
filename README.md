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


