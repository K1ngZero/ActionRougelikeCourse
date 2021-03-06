// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class ActionRougelikeEditorTarget : TargetRules
{
	public ActionRougelikeEditorTarget( TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V2;
		ExtraModuleNames.AddRange( 
            new string[] {
                "ActionRougelike" 
            });

        if (Type == TargetType.Editor)
        {
            ExtraModuleNames.AddRange(
                    new string[] {
                        "ActionRougelikeEditor"
                    });
        }
    }
}
