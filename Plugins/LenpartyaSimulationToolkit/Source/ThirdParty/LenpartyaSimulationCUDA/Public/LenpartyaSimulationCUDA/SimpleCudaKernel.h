#pragma once

#if defined _WIN32 || defined _WIN64
#define LENPARTYACUDA_EXPORT __declspec(dllimport)
#elif defined __linux__
#define LENPARTYACUDA_EXPORT __attribute__((visibility("default")))
#else
#define LENPARTYACUDA_EXPORT
#endif

LENPARTYACUDA_EXPORT void launchKernel();
