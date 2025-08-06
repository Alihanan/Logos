#if defined _WIN32 || defined _WIN64
#include <Windows.h>

#define LENPARTYACUDA_EXPORT __declspec(dllexport)
#else
#include <stdio.h>
#endif

#ifndef LENPARTYACUDA_EXPORT
#define LENPARTYACUDA_EXPORT
#endif


#include <cuda_runtime.h>
#include <iostream>
#include <stdio.h>

__global__ void myKernel()
{
    printf("Hello from CUDA Kernel!\n");
}

LENPARTYACUDA_EXPORT void launchKernel()
{
    myKernel<<<1, 1>>>();
    cudaDeviceSynchronize();

#if defined _WIN32 || defined _WIN64
    MessageBox(NULL, TEXT("Loaded LenpartyaCUDA.dll manually!"), TEXT("Third Party Plugin"), MB_OK);
#else
    printf("Loaded LenpartyaCUDA manually!");
#endif
}
