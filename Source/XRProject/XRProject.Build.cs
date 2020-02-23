// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;

public class XRProject : ModuleRules
{
	public XRProject(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject",
            "InputCore", "AIModule", "NavigationSystem", "GameplayTasks"});

		PrivateDependencyModuleNames.AddRange(new string[] {  });

        if (Target.Type == TargetRules.TargetType.Editor) PrivateDependencyModuleNames.AddRange(new string[] { "Engine", "UMG", "Slate", "SlateCore" });
    }
}
