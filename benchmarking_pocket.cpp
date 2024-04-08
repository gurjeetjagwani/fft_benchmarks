#include <benchmark/benchmark.h> 
#include <complex>     
#include <vector>             
#include "../pocketfft/pocketfft_hdronly.h"

using namespace pocketfft;

static int size;

template<typename T>
static void bench_pocket_unoptimised(benchmark::State& state, shape_t shape, stride_t stride, std::vector<std::complex<T>> c_in, std::vector<std::complex<T>> c_out)
{   
    shape_t axes = size;
    while (state.KeepRunning()){
        c2c(shape, stride, stride, axes, FORWARD, c_in, c_out, 1.);
        benchmark::DoNotOptimize(c_out);
    }
}


template<typename T>
static void bench_pocket_optimised(benchmark::State& state, shape_t shape, stride_t stride, std::vector<std::complex<T>> c_in, std::vector<std::complex<T>> c_out)   
{
    shape_t axes = size;
    while (state.KeepRunning())
    {
        c2c(shape, stride, stride, axes, FORWARD, c_in, c_out, 1.f);
    }
}

template<typename T>
static void benchmark_exec (int argc, char* argv[], int size, std::vector<std::complex<T>> c_in, std::vector<std::complex<T>> c_out)
{
    shape_t shape{size, size};
    if (std::is_same<T,double>::value)
    {
        stride_t strided(shape.size());
        printf("\n Allocating memory ...");
        for (int i = 0; i < size; ++i)
        {
            c_in[i] = {0.3f * i, 10.f * i - sqrtf(i)};
        }
        printf("\n Finished allocating \n");
        benchmark::RegisterBenchmark("Pocket benchmark unoptimised d", bench_pocket_unoptimised<T>, shape, strided, c_in, c_out);
        benchmark::RegisterBenchmark("Pocket benchmark optimised d", bench_pocket_optimised<T>, shape, strided, c_in, c_out);
    }
    else if (std::is_same<T,float>::value)
    {
        stride_t stridef(shape.size());
        printf("\n Allocating memory ...");
        for (int i = 0; i < size; ++i)
        {
            c_in[i] = {0.3f * i, 10.f * i - sqrtf(i)};
        }
        printf("\n Finished allocating \n");
        benchmark::RegisterBenchmark("Pocket benchmark unoptimised f", bench_pocket_unoptimised<T>, shape, stridef, c_in, c_out);
        benchmark::RegisterBenchmark("Pocket benchmark optimised f", bench_pocket_optimised<T>, shape, stridef, c_in, c_out);
    }
}


int main(int argc, char* argv[]){
    typedef double dT;
    size = 23000*23000; 
    std::vector<std::complex<dT>> dc1_in (size);
    std::vector<std::complex<dT>> dc2_out(size);
    benchmark_exec(argc, argv, size, dc1_in, dc2_out);
    benchmark::RunSpecifiedBenchmarks();
}