// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Logos : ModuleRules
{
	public Logos(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		//Target.bUseUnityBuild = false;

        PublicDependencyModuleNames.AddRange(new string[] {
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"AIModule",
			"StateTreeModule",
			"GameplayStateTreeModule",
			"UMG",

            "RHI", "RenderCore" // added
        });

		PrivateDependencyModuleNames.AddRange(new string[] { });

		PublicIncludePaths.AddRange(new string[] {
			"Logos",
			"Logos/Variant_Platforming",
			"Logos/Variant_Combat",
			"Logos/Variant_Combat/AI",
			"Logos/Variant_SideScrolling",
			"Logos/Variant_SideScrolling/Gameplay",
			"Logos/Variant_SideScrolling/AI",
            "Logos/Geometry",
            "Logos/Math"
        });

        PrivateDependencyModuleNames.AddRange(new string[] { "ProceduralMeshComponent" }); // added
        if (Target.bBuildEditor)
        {
            PrivateDependencyModuleNames.Add("UnrealEd");
            PrivateDependencyModuleNames.Add("EditorStyle");
        }
        // Uncomment if you are using Slate UI
        // PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

        // Uncomment if you are using online features
        // PrivateDependencyModuleNames.Add("OnlineSubsystem");

        // To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
    }
}
