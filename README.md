# fft_benchmarks
Benchmarks for Large Scale FFTs


Results on large 2D FFTs from 50k * 50k upto 131k * 131k

Libraries tested:
- fftw
- oneapi:mkl
- pocketfft

build command for fftw benchmarks:
```
g++ -std=c++2a -o benchfftw benchmarking_fftw.cpp -fopenmp -lfftw3_threads -lfftw3_omp -lfftw3 -lm -lpthread -isystem benchmark/include -Lbenchmark/build/src -lbenchmark
```