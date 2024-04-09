#include <benchmark/benchmark.h> 
#include <complex>     
#include <vector>             
#include "../pocketfft/pocketfft_hdronly.h"

using namespace pocketfft;

static int size;

template<typename T>
static void bench_pocket_unoptimised(benchmark::State& state, shape_t shape, stride_t stride, std::vector<std::complex<T>> c_in)
{   
    if(std::is_same<T,float>::value)
    {
        shape_t axes = {0, 1};
        auto c_out = c_in;
        while (state.KeepRunning()){
            c2c(shape, stride, stride, axes, FORWARD, c_in.data(), c_out.data(), 1.f, 128);
            benchmark::DoNotOptimize(c_out.data());
        }
    }
    else if (std::is_same<T,double>::value)
    {
        shape_t axes = {0, 1};
        auto c_out = c_in;
        while (state.KeepRunning()){
            c2c(shape, stride, stride, axes, FORWARD, c_in.data(), c_out.data(), 1., 128);
            benchmark::DoNotOptimize(c_out.data());
        }
    }

}


template<typename T>
static void bench_pocket_optimised(benchmark::State& state, shape_t shape, stride_t stride, std::vector<std::complex<T>> c_in)   
{
    if(std::is_same<T,float>::value)
    {
        shape_t axes = {0, 1};
        auto c_out = c_in;
        while (state.KeepRunning()){
            c2c(shape, stride, stride, axes, FORWARD, c_in.data(), c_out.data(), 1.f, 128);
        }
    }
    else if (std::is_same<T,double>::value)
    {
        shape_t axes = {0, 1};
        auto c_out = c_in;
        while (state.KeepRunning()){
            c2c(shape, stride, stride, axes, FORWARD, c_in.data(), c_out.data(), 1., 128);
        }
    }
}

template<typename T>
static void benchmark_exec (int argc, char* argv[], shape_t shape, std::vector<std::complex<T>> c_in)
{

    if (std::is_same<T,double>::value)
    {
        stride_t strided(shape.size());
        printf("\n Allocating memory ...");
        for (int i = 0; i < (int)shape.size(); i++)
        {
            c_in[i] = {0.3f * i, 10.f * i - sqrtf(i)};
        }
        printf("\n Finished allocating \n");
        benchmark::RegisterBenchmark("Pocket benchmark unoptimised d", bench_pocket_unoptimised<T>, shape, strided, c_in);
        benchmark::RegisterBenchmark("Pocket benchmark optimised d", bench_pocket_optimised<T>, shape, strided, c_in);
        benchmark::Initialize(&argc, argv);
    }
    else if (std::is_same<T,float>::value)
    {
        stride_t stridef(shape.size());
        printf("\n Allocating memory ...");
        for (int i = 0; i < (int)shape.size(); i++)
        {
            c_in[i] = {0.3f * i, 10.f * i - sqrtf(i)};
        }
        printf("\n Finished allocating \n");
        benchmark::RegisterBenchmark("Pocket benchmark unoptimised f", bench_pocket_unoptimised<T>, shape, stridef, c_in);
        benchmark::RegisterBenchmark("Pocket benchmark optimised f", bench_pocket_optimised<T>, shape, stridef, c_in);
        benchmark::Initialize(&argc, argv);
    }
}

int main(int argc, char* argv[]){
    typedef float dT;
    size_t shape1, shape2 = 23000;
    shape_t shape{shape1, shape2}; 
    std::vector<std::complex<dT>> dc1_in ((int)shape.size());
    benchmark_exec<dT>(argc, argv, shape, dc1_in);
    benchmark::RunSpecifiedBenchmarks();
}