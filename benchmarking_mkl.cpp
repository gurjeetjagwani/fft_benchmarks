#include <vector>
#include <complex>
#include "/opt/intel/oneapi/mkl/2024.1/include/fftw/fftw3_mkl.h"
#include <fftw3.h>
#include <omp.h>
#include <benchmark/benchmark.h>

static long size1;
static long size2;

template<typename T>
static void bench_mkl_unoptimized(benchmark::State& state, std::vector<std::complex<T>> c1)
{   
  //Output vector
    std::vector<std::complex<T>> c2(size1 * size2);
    
    if(std::is_same<T,double>::value)
    {   
        fftw_plan_with_nthreads(omp_get_max_threads());
        fftw_plan plan = fftw_plan_dft_2d(size1, size2, (fftw_complex*)(&c1[0]), (fftw_complex*)(&c2[0]), FFTW_FORWARD, FFTW_MEASURE);

        while(state.KeepRunning()){
            fftw_execute(plan);
            benchmark::DoNotOptimize(c2);
        }
        
        fftw_destroy_plan(plan);
        fftw_cleanup_threads();
    }
    else if (std::is_same<T,float>::value)
    {
        fftwf_plan_with_nthreads(omp_get_max_threads());
        fftwf_plan plan = fftwf_plan_dft_2d(size1, size2, reinterpret_cast<fftwf_complex*>(&c1[0]), reinterpret_cast<fftwf_complex*>(&c2[0]), FFTW_FORWARD, FFTW_MEASURE);

        while(state.KeepRunning()){
            fftwf_execute(plan);
            benchmark::DoNotOptimize(c2);
        }
        fftwf_destroy_plan(plan);
        fftwf_cleanup_threads();
    }    
}

template<typename T>
static void benchmark_mkl_optimised(benchmark::State& state, std::vector<std::complex<T>> c1)
{
  //Output vector
    std::vector<std::complex<T>> c2(size1 * size2);
    
    if(std::is_same<T,double>::value)
    {   
        fftw_plan_with_nthreads(omp_get_max_threads());
        fftw_plan plan = fftw_plan_dft_2d(size1, size2, (fftw_complex*)(&c1[0]), (fftw_complex*)(&c2[0]), FFTW_FORWARD, FFTW_MEASURE);

        while(state.KeepRunning()){
            fftw_execute(plan);
        }
        
        fftw_destroy_plan(plan);
        fftw_cleanup_threads();
    }
    else if (std::is_same<T,float>::value)
    {
        fftwf_plan_with_nthreads(omp_get_max_threads());
        fftwf_plan plan = fftwf_plan_dft_2d(size1, size2, reinterpret_cast<fftwf_complex*>(&c1[0]), reinterpret_cast<fftwf_complex*>(&c2[0]), FFTW_FORWARD, FFTW_MEASURE);

        while(state.KeepRunning()){
            fftwf_execute(plan);
        }
        fftwf_destroy_plan(plan);
        fftwf_cleanup_threads();
    }    
}

template <typename T>
static void benchmark_exec(int argc, char* argv[], int size1, int size2, std::vector<std::complex<T>> c1)
{
    printf("\n Allocating memory ...");
    for (int i = 0; i < size1 * size2; ++i)
    {
        c1[i] = {0.3f * i, 10.f * i - sqrtf(i)};
    }
    printf("\n Finished allocating \n");
    
    if(std::is_same<T,double>::value)
    {
        fftw_init_threads();
        benchmark::RegisterBenchmark("MKL multi threaded unoptmised d", bench_mkl_unoptimized<T>, c1);
        benchmark::RegisterBenchmark("MKL multi threaded optimised d", benchmark_mkl_optimised<T>, c1);
    }
    else if (std::is_same<T,float>::value)
    {
        fftwf_init_threads();
        benchmark::RegisterBenchmark("MKL multi threaded unoptmised f", benchmark_mkl_optimised<T>, c1);
        benchmark::RegisterBenchmark("MKL multi threaded optimised f", benchmark_mkl_optimised<T>, c1);
    }
    benchmark::Initialize(&argc, argv);
}


int main (int argc, char* argv[])
{
    fftw3_mkl.verbose = 0;
    typedef double dT;
    size1 = 23000;
    size2 = 23000;
    printf("Epoch 1: Allocating %ld x %ld benchmarks on complex double\n", size1, size2);
    std::vector<std::complex<dT>> dc1 (size1 * size2);
    benchmark_exec<dT>(argc, argv, size1, size2, dc1);
    typedef float fT;
    printf("Epoch 2: Allocating %ld X %ld benchmarks on complex float\n", size1, size2);
    std::vector<std::complex<fT>> fc1 (size1 * size2);
    benchmark_exec<fT>(argc, argv, size1, size2, fc1);
    benchmark::RunSpecifiedBenchmarks();
}