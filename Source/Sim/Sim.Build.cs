// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Sim : ModuleRules
{
	public Sim(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay" });
	}
}
