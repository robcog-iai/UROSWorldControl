// Copyright 2018, Institute for Artificial Intelligence - University of Bremen

#pragma once

#include "CoreMinimal.h"
#include "ModuleManager.h"
#include "Styling/ISlateStyle.h"
#include "Styling/SlateStyle.h"

class FUROSWorldControlEd : public IModuleInterface
{
public:
	/** IModuleInterface implementation */
	void StartupModule() override;
	void ShutdownModule() override;

	// Init style
	static void Initialize();

	// Get instance
	static TSharedPtr<class ISlateStyle> Get();

	// Get style name
	static FName GetStyleSetName();

private:
	// Style instance
	static TSharedPtr<class FSlateStyleSet> StyleSetInstance;
};
