// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class ProjectFL : ModuleRules
{
	public ProjectFL(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { 
			"Core", "CoreUObject", "Engine", "InputCore", 
			"HeadMountedDisplay", "EnhancedInput", 
			"GeometryCollectionEngine", "UMG", "Slate", "SlateCore", "ALSV4_CPP" });

		PublicIncludePaths.AddRange(new string[] { "ALSV4_CPP/Public", "ALSV4_CPP/Classes" });

	}
}
