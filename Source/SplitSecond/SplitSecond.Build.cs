// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class SplitSecond : ModuleRules
{
	public SplitSecond(ReadOnlyTargetRules Target) : base(Target)
	{
		PrecompileForTargets = PrecompileTargetsType.Any;

		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "AIModule", "Niagara", "NavigationSystem", "UMG", "Slate", "SlateCore" });
	}
}
