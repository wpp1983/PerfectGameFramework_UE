// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class PerfectGameFramework : ModuleRules
{
	public PerfectGameFramework(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicIncludePaths.AddRange(
			new string[] {
				// ... add public include paths required here ...
			}
			);
		
		
		PrivateIncludePaths.AddRange(
			new string[] {
				// ... add other private include paths required here ...
			}
			);
			
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				// ... add other public dependencies that you statically link with here ...
                "ModularGameplay",
                "UIExtension",

			}
			);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore", 
				"PhysicsCore",
				"GameFeatures", 
				"GameplayAbilities",
				"DeveloperSettings",
				"GameplayTasks",
				"GameplayTags", 
				"EnhancedInput",
				"GameplayMessageRuntime", 
				"CommonGame",
                "ModularGameplayActors",
                "Niagara",
                "NetCore", 
                "SignificanceManager",
                "CommonUI",
                "EngineSettings",
                "UMG",
                "AsyncMixin",
                "ControlFlows",
                "CommonUser",
                "CommonInput",
                "InputCore",
                "AudioModulation",
                "AIModule",
				
				// ... add private dependencies that you statically link with here ...	
			}
			);
		
		
		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				// ... add any modules that your module loads dynamically here ...
			}
			);
		
		SetupGameplayDebuggerSupport(Target);
		SetupIrisSupport(Target);
	}
}
