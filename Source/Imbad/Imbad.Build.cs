// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Imbad : ModuleRules
{
	public Imbad(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "EnhancedInput" });

		// AI Dependencies
		PrivateDependencyModuleNames.AddRange(new string[] { "AIModule", "NavigationSystem", "Niagara", "ProceduralDungeon" });
		
		// Gameplay Ability System Dependencies
		PublicDependencyModuleNames.AddRange(new string[] { "GameplayAbilities", "GameplayTags", "GameplayTasks", "ProceduralDungeon", "SlateCore" });
	}
}
