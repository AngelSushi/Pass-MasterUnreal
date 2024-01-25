// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class PassMaster : ModuleRules
{
	public PassMaster(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput" });
	}
}
