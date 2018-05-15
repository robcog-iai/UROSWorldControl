// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "UnrealWorldControlEdModeModule.h"
#include "UnrealWorldControlEdMode.h"
#include "UnrealWorldControlEdTool.h"

#include "PropertyEditorModule.h"

#include "Styling/SlateStyleRegistry.h"
#include "SlateOptMacros.h"

#define LOCTEXT_NAMESPACE "FUnrealWorldControlEdModeModule"
#define IMAGE_PLUGIN_BRUSH( RelativePath, ...) FSlateImageBrush (FUnrealWorldControlEdModeModule::InContent( RelativePath, ".png"), __VA_ARGS__)

FString FUnrealWorldControlEdModeModule::InContent(const FString& RelativePath, const ANSICHAR* Extension)
{
	static FString ContentDir = FPaths::ProjectPluginsDir() / TEXT("UROSWorldControl/Content/Slate");
	return (ContentDir / RelativePath) + Extension;
}

TSharedPtr<FSlateStyleSet> FUnrealWorldControlEdModeModule::StyleSet = nullptr;
TSharedPtr<class ISlateStyle> FUnrealWorldControlEdModeModule::Get() { return StyleSet; }

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void FUnrealWorldControlEdModeModule::Initialize()
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
	StyleSet->SetContentRoot(FPaths::ProjectPluginsDir() / TEXT("UnrealWorldControlEdMode/Content/Slate"));
	StyleSet->SetCoreContentRoot(FPaths::EngineContentDir() / TEXT("Slate"));

	StyleSet->Set("LevelEditor.UnrealWorldControlEdMode", new IMAGE_PLUGIN_BRUSH("Icons/Icon128", Icon40x40));
	StyleSet->Set("LevelEditor.UnrealWorldControlEdMode.Small", new IMAGE_PLUGIN_BRUSH("Icons/Icon128", Icon20x20));
	StyleSet->Set("LevelEditor.UnrealWorldControlEdMode.Selected", new IMAGE_PLUGIN_BRUSH("Icons/Icon128", Icon40x40));
	StyleSet->Set("LevelEditor.UnrealWorldControlEdMode.Selected.Small",
	              new IMAGE_PLUGIN_BRUSH("Icons/Icon128", Icon20x20));


	FSlateStyleRegistry::RegisterSlateStyle(*StyleSet.Get());
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION

#undef IMAGE_PLUGIN_BRUSH

FName FUnrealWorldControlEdModeModule::GetStyleSetName()
{
	static FName StyleName(TEXT("UnrealWorldControlEdMode"));
	return StyleName;
}

void FUnrealWorldControlEdModeModule::StartupModule()
{
	//Initialize styleset
	Initialize();

	//register mode
	FEditorModeRegistry::Get().RegisterMode<FUnrealWorldControlEdMode>(
		FUnrealWorldControlEdMode::EM_UnrealWorldControlEdModeId,
		LOCTEXT("UnrealWorldControlEdModeEdModeName", "UnrealWorldControlEdModeEdMode"),
		FSlateIcon(GetStyleSetName(), "LevelEditor.UnrealWorldControlEdMode", "LevelEditor.UnrealWorldControlEdMode.Small"),
		true
	);
}

void FUnrealWorldControlEdModeModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	FEditorModeRegistry::Get().UnregisterMode(FUnrealWorldControlEdMode::EM_UnrealWorldControlEdModeId);
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FUnrealWorldControlEdModeModule, UnrealWorldControlEdMode)
