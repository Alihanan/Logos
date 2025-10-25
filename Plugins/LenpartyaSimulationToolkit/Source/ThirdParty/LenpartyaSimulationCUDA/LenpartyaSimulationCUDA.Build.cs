// Fill out your copyright notice in the Description page of Project Settings.

using System.IO;
using UnrealBuildTool;

public class LenpartyaSimulationCUDA : ModuleRules
{
	public LenpartyaSimulationCUDA(ReadOnlyTargetRules Target) : base(Target)
	{
		Type = ModuleType.External;
		PublicSystemIncludePaths.Add("$(ModuleDir)/Public");

		if (Target.Platform == UnrealTargetPlatform.Win64)
		{
			// Add the import library
			PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "x64", "Release", "LenpartyaSimulationCUDA.lib"));

			// Delay-load the DLL, so we can load it from the right place first
			PublicDelayLoadDLLs.Add("LenpartyaSimulationCUDA.dll");

			// Ensure that the DLL is staged along with the executable
			RuntimeDependencies.Add("$(PluginDir)/Binaries/Win64/LenpartyaSimulationCUDA.dll");
		}
		else if (Target.Platform == UnrealTargetPlatform.Mac)
		{
			PublicDelayLoadDLLs.Add(Path.Combine(ModuleDirectory, "Mac", "Release", "libLenpartyaSimulationCUDA.dylib"));
			RuntimeDependencies.Add("$(PluginDir)/Source/ThirdParty/LenpartyaSimulationCUDA/Mac/Release/libLenpartyaSimulationCUDA.dylib");
		}
		else if (Target.Platform == UnrealTargetPlatform.Linux)
		{
			string ExampleSoPath = Path.Combine("$(PluginDir)", "Binaries", "Linux", "x86_64-unknown-linux-gnu", "libLenpartyaSimulationCUDA.so");
			PublicAdditionalLibraries.Add(ExampleSoPath);
			PublicDelayLoadDLLs.Add(ExampleSoPath);
			RuntimeDependencies.Add(ExampleSoPath);
		}
	}
}
