// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "RosConnectionModeModule.h"
#include "RosConnectionMode.h"
#include "RosConnectionToolCustomization.h"
#include "RosConnectionTool.h"

#include "PropertyEditorModule.h"
#include "Interfaces/IPluginManager.h"

#include "Styling/SlateStyleRegistry.h"
#include "Styling/SlateTypes.h"
#include "EditorStyleSet.h"
#include "SlateOptMacros.h"

#define LOCTEXT_NAMESPACE "FRosConnectionModeModule"
#define IMAGE_PLUGIN_BRUSH( RelativePath, ... ) FSlateImageBrush( FRosConnectionModeModule::InContent( RelativePath, ".png" ), __VA_ARGS__ )

FString FRosConnectionModeModule::InContent(const FString & RelativePath, const ANSICHAR * Extension)
{
	static FString ContentDir = FPaths::ProjectPluginsDir() / TEXT("UROSWorldControl/Content/Slate");
	return (ContentDir / RelativePath) + Extension;
}

TSharedPtr< FSlateStyleSet > FRosConnectionModeModule::StyleSet = nullptr;
TSharedPtr< class ISlateStyle > FRosConnectionModeModule::Get() { return StyleSet; }

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void FRosConnectionModeModule::Initialize()
{
	// Const icon sizes
	const FVector2D Icon8x8(8.0f, 8.0f);
	const FVector2D Icon16x16(16.0f, 16.0f);
	const FVector2D Icon20x20(20.0f, 20.0f);
	const FVector2D Icon40x40(40.0f, 40.0f);

	// Only register once
	if (StyleSet.IsValid())
	{
		return;
	}

	StyleSet = MakeShareable(new FSlateStyleSet(GetStyleSetName()));
	StyleSet->SetContentRoot(FPaths::ProjectPluginsDir() / TEXT("RosConnectionMode/Content/Slate"));
	StyleSet->SetCoreContentRoot(FPaths::EngineContentDir() / TEXT("Slate"));

	StyleSet->Set("LevelEditor.RosConnectionMode", new IMAGE_PLUGIN_BRUSH("Icons/Icon128", Icon40x40));
	StyleSet->Set("LevelEditor.RosConnectionMode.Small", new IMAGE_PLUGIN_BRUSH("Icons/Icon128", Icon20x20));
	StyleSet->Set("LevelEditor.RosConnectionMode.Selected", new IMAGE_PLUGIN_BRUSH("Icons/Icon128", Icon40x40));
	StyleSet->Set("LevelEditor.RosConnectionMode.Selected.Small", new IMAGE_PLUGIN_BRUSH("Icons/Icon128", Icon20x20));


	FSlateStyleRegistry::RegisterSlateStyle(*StyleSet.Get());
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION

#undef IMAGE_PLUGIN_BRUSH

FName FRosConnectionModeModule::GetStyleSetName()
{
	static FName StyleName(TEXT("RosConnectionMode"));
	return StyleName;
}

void FRosConnectionModeModule::StartupModule()
{
	//Initialize styleset
	Initialize();

	//register mode
	FEditorModeRegistry::Get().RegisterMode<FRosConnectionMode>(
		FRosConnectionMode::EM_RosConnectionModeId,
		LOCTEXT("RosConnectionModeEdModeName", "RosConnectionModeEdMode"),
		FSlateIcon(GetStyleSetName(), "LevelEditor.RosConnectionMode", "LevelEditor.RosConnectionMode.Small"),
		true
		);
}

void FRosConnectionModeModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	FEditorModeRegistry::Get().UnregisterMode(FRosConnectionMode::EM_RosConnectionModeId);

}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FRosConnectionModeModule, RosConnectionMode)