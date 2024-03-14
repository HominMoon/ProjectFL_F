// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class ProjectFLEditorTarget : TargetRules
{
	public ProjectFLEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V2;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_1;
        CppStandard = CppStandardVersion.Cpp20;
        ExtraModuleNames.Add("ProjectFL");
	}
}
