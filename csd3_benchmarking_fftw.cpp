#include <vector>
#include <complex>
#include <fftw3.h>
#include <omp.h>
#include "CPU_timer.h"

CPU_Timer cpu;

static void run_fftw3_double(int size1, int size2, std::vector<std::complex<double>> complex_in)
{
    fftw_init_threads();
    printf("\n Allocating memory ...");
    for (int i = 0; i < size1 * size2; ++i)
    {
        complex_in[i] = {0.3f * i, 10.f * i - sqrtf(i)};
    }
    printf("\n Finished allocating \n");
    //Output vector
    std::vector<std::complex<double>> complex_out (size1 * size2); 
    fftw_plan_with_nthreads(omp_get_max_threads());
    fftw_plan plan = fftw_plan_dft_2d(size1, size2, reinterpret_cast<fftw_complex*>(&complex_in[0]), reinterpret_cast<fftw_complex*>(&complex_out[0]), FFTW_FORWARD, FFTW_MEASURE);
    cpu.Start();
    fftw_execute(plan);
    cpu.Stop();
    printf("Double precision time FFTW3 2D = %e", cpu.Elapsed());
    fftw_destroy_plan(plan);
    fftw_cleanup_threads();
}

static void run_fftw3_single(int size1, int size2, std::vector<std::complex<float>> complex_in)
{
    
    fftwf_init_threads();
    printf("\n Allocating memory ...");
    for (int i = 0; i < size1 * size2; ++i)
    {
        complex_in[i] = {0.3f * i, 10.f * i - sqrtf(i)};
    }
    printf("\n Finished allocating \n");
    //Output vector
    std::vector<std::complex<float>> complex_out (size1 * size2);
    fftwf_plan_with_nthreads(omp_get_max_threads());
    fftwf_plan plan = fftwf_plan_dft_2d(size1, size2, reinterpret_cast<fftwf_complex*>(&complex_in[0]), reinterpret_cast<fftwf_complex*>(&complex_out[0]), FFTW_FORWARD, FFTW_MEASURE);
    cpu.Start();
    fftwf_execute(plan);
    cpu.Stop();
    printf("Single precision time FFTW3 2D = %e", cpu.Elapsed());
    fftwf_destroy_plan(plan);
    fftwf_cleanup_threads();
}

int main()
{
    int size1 = 32000;
    int size2 = 32000;
    //Input vector double precision
    std::vector<std::complex<double>> complex_in_d (size1 * size2);
    //Input vector single precision
    std::vector<std::complex<float>> complex_in_f (size1 * size2);
    run_fftw3_double(size1, size2, complex_in_d);
    run_fftw3_single(size1, size2, complex_in_f);
}