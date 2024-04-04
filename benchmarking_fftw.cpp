#include <iostream>
#include <fftw3.h>
#include <benchmark/benchmark.h>
#include <complex>
#include <vector>
#include <omp.h>

static int size1;
static int size2;

static void s_bench_fftw3_unoptimized(benchmark::State& state, std::vector<std::complex<double>> c1)
{   
    //Output vector
    std::vector<std::complex<double>> c2(size1 * size2);

    fftw_plan plan = fftw_plan_dft_2d(size1, size2, reinterpret_cast<fftw_complex*>(&c1[0]), reinterpret_cast<fftw_complex*>(&c2[0]), FFTW_FORWARD, FFTW_MEASURE);

    while(state.KeepRunning()){
        fftw_execute(plan);
        benchmark::DoNotOptimize(c2);
    }
    fftw_destroy_plan(plan);    
}

static void s_bench_fftw3_optimized(benchmark::State& state, std::vector<std::complex<double>> c1)
{
    //Output vector
    std::vector<std::complex<double>> c2(size1 * size2);

    fftw_plan plan = fftw_plan_dft_2d(size1, size2, reinterpret_cast<fftw_complex*>(&c1[0]), reinterpret_cast<fftw_complex*>(&c2[0]), FFTW_FORWARD, FFTW_MEASURE);

    while(state.KeepRunning()){
        fftw_execute(plan);
    }
    fftw_destroy_plan(plan);    
}

static void m_bench_fftw3_unoptimized(benchmark::State& state, std::vector<std::complex<double>> c1 )
{
    //Output vector
    std::vector<std::complex<double>> c2(size1 * size2);

    fftw_plan_with_nthreads(omp_get_max_threads());

    fftw_plan plan = fftw_plan_dft_2d(size1, size2, (fftw_complex*)(&c1[0]), (fftw_complex*)(&c2[0]), FFTW_FORWARD, FFTW_MEASURE);

    while(state.KeepRunning()){
        fftw_execute(plan);
        benchmark::DoNotOptimize(c2);
    }
    fftw_destroy_plan(plan);
    fftw_cleanup_threads();

}

static void m_bench_fftw3_optimized(benchmark::State& state, std::vector<std::complex<double>> c1)
{
    //Output vector
    std::vector<std::complex<double>> c2 (size1 * size2);

    fftw_plan_with_nthreads(omp_get_max_threads());

    fftw_plan plan = fftw_plan_dft_2d(size1, size2, (fftw_complex*)(&c1[0]), (fftw_complex*)(&c2[0]), FFTW_FORWARD, FFTW_MEASURE);
    
    while (state.KeepRunning()){
        fftw_execute(plan);
    }
    fftw_destroy_plan(plan);
    fftw_cleanup_threads();    
}

static void benchmark_exec(int argc, char* argv[], int size1, int size2, std::vector<std::complex<double>> c1)
{
        printf("\n Allocating memory ...");
        for (int i = 0; i < size1 * size2; ++i)
        {
            c1[i] = {0.3f * i, 10.f * i - sqrtf(i)};
        }
        printf("\n Finished allocating");
        //Avoid single threaded benchmarks for now
        //benchmark::RegisterBenchmark("FFTW3 single threaded unoptimised", s_bench_fftw3_unoptimized);
        //benchmark::RegisterBenchmark("FFTW3 single threaded optimised", s_bench_fftw3_optimized); 
        fftw_init_threads();
        benchmark::RegisterBenchmark("FFTW3 multi threaded unoptmised", m_bench_fftw3_unoptimized, c1);
        benchmark::RegisterBenchmark("FFTW3 multi threaded optimised", m_bench_fftw3_optimized, c1);
        benchmark::Initialize(&argc, argv);
        benchmark::RunSpecifiedBenchmarks();

}


int main(int argc, char* argv[])
{ 
       
    printf("Epoch 1: Executing 32k x 32k benchmarks \n");
    size1 = 4096;
    size2 = 4096;
    std::vector<std::complex<double>> c1 (size1 * size2);
    benchmark_exec(argc, argv, size1, size2, c1);
    
    printf("\n Epoch 2: Executing 65k * 65k benchmarks \n");
    size1 = 8192;
    size2 = 8192;
    std::vector<std::complex<double>> c2 (size1 * size2);
    benchmark_exec(argc, argv, size1, size2, c2);
    
    // printf("\n Epoch 3: Executing 131k * 131k benchmarks \n");
    // size1 = 131072;
    // size2 = 131072;
    // benchmark_exec(argc, argv, size1, size2);
}
