// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class IN : ModuleRules
{
	public IN(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "MACRO_MASTER",
        "Slate", "SlateCore", "AIModule", "StateTreeModule", "GameplayStateTreeModule", "NavigationSystem", "GameplayTags", "AnimationBudgetAllocator"});

		PrivateDependencyModuleNames.AddRange(new string[] { "AnimGraphRuntime" });

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
