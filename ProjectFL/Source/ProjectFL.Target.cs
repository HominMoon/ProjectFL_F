// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class ProjectFLTarget : TargetRules
{
	public ProjectFLTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V2;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_1;
		CppStandard = CppStandardVersion.Cpp20;
		ExtraModuleNames.Add("ProjectFL");
		
	}
}
