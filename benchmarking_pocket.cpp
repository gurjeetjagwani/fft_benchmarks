#include <benchmark/benchmark.h> 
#include <iostream>
#include <complex>     
#include <vector>             
#include "../pocketfft/pocketfft_hdronly.h"

using namespace pocketfft;

static int size;

template<typename T>
static void bench_pocket_f_unoptimised(benchmark::State& state, shape_t shape, stride_t stride, std::vector<std::complex<T>> c_in, std::vector<std::complex<T>> c_out)
{   
    shape_t axes = {0, 1};
    while (state.KeepRunning()){
        c2c<T>(shape, stride, stride, axes, FORWARD, c_in.data(), c_out.data(), 1.f, 128);
        benchmark::DoNotOptimize(c_out.data());
    }
}

template<typename T>
static void bench_pocket_f_optimised(benchmark::State& state, shape_t shape, stride_t stride, std::vector<std::complex<T>> c_in, std::vector<std::complex<T>> c_out)   
{
    shape_t axes = {0, 1};
    while (state.KeepRunning()){
        c2c<T>(shape, stride, stride, axes, FORWARD, c_in.data(), c_out.data(), 1.f, 128);
    }
}

template<typename T>
static void bench_pocket_d_unoptimised(benchmark::State& state, shape_t shape, stride_t stride, std::vector<std::complex<T>> c_in, std::vector<std::complex<T>> c_out)   
{
    shape_t axes = {0, 1};
    while (state.KeepRunning()){
        c2c<T>(shape, stride, stride, axes, FORWARD, c_in.data(), c_out.data(), 1. , 128);
        benchmark::DoNotOptimize(c_out.data());
    }
}

template<typename T>
static void bench_pocket_d_optimised(benchmark::State& state, shape_t shape, stride_t stride, std::vector<std::complex<T>> c_in, std::vector<std::complex<T>> c_out)   
{
    shape_t axes = {0, 1};
    while (state.KeepRunning()){
        c2c<T>(shape, stride, stride, axes, FORWARD, c_in.data(), c_out.data(), 1. , 128);
    }
}

template<typename T>
static void benchmark_exec(int argc, char* argv[], shape_t shape, size_t shape1, size_t shape2, std::vector<std::complex<T>> c_in, std::vector<std::complex<T>> c_out)
{
    printf("\n Allocating memory input...");
    for (int i = 0; i < (int)shape.size(); i++)
    {
        c_in[i] = {0.3f * i, 10.f * i - sqrtf(i)};
    }
    printf("\n Finished allocating \n");

    printf("\n Allocating memory output ...");
    for (int i = 0; i < (int)shape.size(); i++)
    {
        c_out[i] = {0.5f * i, 10.f * i - sqrtf(i)};
    }
    printf("\n Finished allocating \n");

    if (std::is_same<T,double>::value)
    {
        stride_t strided{sizeof(c_in[0]), sizeof(c_in[0])}; 
        benchmark::RegisterBenchmark("Pocket benchmark unoptimised d", bench_pocket_d_unoptimised<T>, shape, strided, c_in, c_out);
        benchmark::RegisterBenchmark("Pocket benchmark optimised d", bench_pocket_d_optimised<T>, shape, strided, c_in, c_out);
    }
    else if (std::is_same<T,float>::value)
    {
        stride_t stridef{sizeof(c_in[0]), sizeof(c_in[0])};
        benchmark::RegisterBenchmark("Pocket benchmark unoptimised f", bench_pocket_f_unoptimised<T>, shape, stridef, c_in, c_out);
        benchmark::RegisterBenchmark("Pocket benchmark optimised f", bench_pocket_f_optimised<T>, shape, stridef, c_in, c_out);
    }
    benchmark::Initialize(&argc, argv);
}

int main(int argc, char* argv[]){
    typedef float dT;
    size_t shape1 = 23000;
    size_t shape2 = 23000;
    shape_t shape {shape1, shape2}; 
    int size = shape1 * shape2;
    std::vector<std::complex<dT>> fc1_in (size);
    std::vector<std::complex<dT>> fc1_out (size);
    benchmark_exec<dT>(argc, argv, shape, shape1, shape2,fc1_in, fc1_out);
    benchmark::RunSpecifiedBenchmarks();
}