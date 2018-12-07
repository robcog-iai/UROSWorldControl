// Copyright 2018, Institute for Artificial Intelligence - University of Bremen

#include "UROSWorldControlEd.h"
#include "RWCEdMode.h"
#include "RWCEdTool.h"
#include "PropertyEditorModule.h"
#include "Styling/SlateStyleRegistry.h"
#include "SlateOptMacros.h"
#include "Interfaces/IPluginManager.h"

#define LOCTEXT_NAMESPACE "FUROSWorldControlEd"

TSharedPtr<FSlateStyleSet> FUROSWorldControlEd::StyleSetInstance = nullptr;

void FUROSWorldControlEd::StartupModule()
{
	// Initialize slate style for the module
	Initialize();

	// Register mode
	FEditorModeRegistry::Get().RegisterMode<FRWCEdMode>(
		FRWCEdMode::EM_RWCEdModeId,
		LOCTEXT("UROSWorldControlEd", "ROS World Control"),
		FSlateIcon(GetStyleSetName(), "LevelEditor.UROSWorldControlEd", "LevelEditor.UROSWorldControlEd.Small"),
		true
		);
}

void FUROSWorldControlEd::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	FEditorModeRegistry::Get().UnregisterMode(FRWCEdMode::EM_RWCEdModeId);
}

// Get style instance
TSharedPtr<class ISlateStyle> FUROSWorldControlEd::Get() 
{ 
	return StyleSetInstance; 
}

#define IMAGE_PLUGIN_BRUSH(RelativePath, ...) FSlateImageBrush(StyleSetInstance->RootToContentDir(RelativePath, TEXT(".png")), __VA_ARGS__)
BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void FUROSWorldControlEd::Initialize()
{
	// Const icon sizes
	const FVector2D Icon8x8(8.0f, 8.0f);
	const FVector2D Icon16x16(16.0f, 16.0f);
	const FVector2D Icon20x20(20.0f, 20.0f);
	const FVector2D Icon40x40(40.0f, 40.0f);

	// Only register once
	if (StyleSetInstance.IsValid())
	{
		return;
	}

	StyleSetInstance = MakeShareable(new FSlateStyleSet(GetStyleSetName()));
	StyleSetInstance->SetContentRoot(IPluginManager::Get().FindPlugin(TEXT("UROSWorldControl"))->GetContentDir());
	//StyleSetInstance->SetCoreContentRoot(FPaths::EngineContentDir()));
	
	StyleSetInstance->Set("LevelEditor.UROSWorldControlEd", new IMAGE_PLUGIN_BRUSH(TEXT("Icons/IconMode40px"), Icon40x40));
	StyleSetInstance->Set("LevelEditor.UROSWorldControlEd.Small", new IMAGE_PLUGIN_BRUSH(TEXT("Icons/IconMode40px"), Icon20x20));
	StyleSetInstance->Set("LevelEditor.UROSWorldControlEd.Selected", new IMAGE_PLUGIN_BRUSH(TEXT("Icons/IconMode40px"), Icon40x40));
	StyleSetInstance->Set("LevelEditor.UROSWorldControlEd.Selected.Small", new IMAGE_PLUGIN_BRUSH(TEXT("Icons/IconMode40px"), Icon20x20));

	FSlateStyleRegistry::RegisterSlateStyle(*StyleSetInstance.Get());
}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION
#undef IMAGE_PLUGIN_BRUSH

FName FUROSWorldControlEd::GetStyleSetName()
{
	static FName StyleName(TEXT("UROSWorldControlEdMode"));
	return StyleName;
}



#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FUROSWorldControlEd, RWCEdMode)
