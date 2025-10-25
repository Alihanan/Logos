// Copyright Epic Games, Inc. All Rights Reserved.

#include "LenpartyaSimulationToolkit.h"
#include "Misc/MessageDialog.h"
#include "Modules/ModuleManager.h"
#include "Interfaces/IPluginManager.h"
#include "Misc/Paths.h"
#include "HAL/PlatformProcess.h"
//#include "LenpartyaSimulationToolkitLibrary/ExampleLibrary.h"
#include "LenpartyaSimulationCUDA/SimpleCudaKernel.h"
#include "LenpartyaSimulationCUDA/kernel.h"
#include "LenpartyaSimulationCUDA/icosphere.h"

#define LOCTEXT_NAMESPACE "FLenpartyaSimulationToolkitModule"

void FLenpartyaSimulationToolkitModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module

	// Get the base directory of this plugin
	FString BaseDir = IPluginManager::Get().FindPlugin("LenpartyaSimulationToolkit")->GetBaseDir();

	// Add on the relative location of the third party dll and load it
	FString LibraryPath;

#if PLATFORM_WINDOWS
	LibraryPath = FPaths::Combine(*BaseDir, TEXT("Binaries/Win64/LenpartyaSimulationCUDA.dll"));
#elif PLATFORM_MAC
	LibraryPath = FPaths::Combine(*BaseDir, TEXT("Source/Mac/Release/libExampleLibrary.dylib"));
#elif PLATFORM_LINUX
	LibraryPath = FPaths::Combine(*BaseDir, TEXT("Binaries/Linux/x86_64-unknown-linux-gnu/libExampleLibrary.so"));
#endif // PLATFORM_WINDOWS

	CUDAHandle = !LibraryPath.IsEmpty() ? FPlatformProcess::GetDllHandle(*LibraryPath) : nullptr;

	if (CUDAHandle)
	{
		// Call the test function in the third party library that opens a message box
		launchKernel();
		kernelCU();
	}
	else
	{
		FMessageDialog::Open(EAppMsgType::Ok, LOCTEXT("CUDA library error!", "Failed to load example third party library"));
	}
}

void FLenpartyaSimulationToolkitModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	// Free the dll handle
	FPlatformProcess::FreeDllHandle(ExampleLibraryHandle);
	ExampleLibraryHandle = nullptr;
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FLenpartyaSimulationToolkitModule, LenpartyaSimulationToolkit)
