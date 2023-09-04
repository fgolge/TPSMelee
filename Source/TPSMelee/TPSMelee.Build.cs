// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class TPSMelee : ModuleRules
{
	public TPSMelee(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "EnhancedInput" , "AnimGraphRuntime", "GameplayTasks" , "MotionWarping"});
		
		PrivateDependencyModuleNames.AddRange(new string[] { "AIModule", "NavigationSystem" });
	}
}
