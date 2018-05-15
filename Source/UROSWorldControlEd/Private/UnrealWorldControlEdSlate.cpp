// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "UnrealWorldControlEdSlate.h"
#include "UnrealWorldControlEdMode.h"
#include "UnrealWorldControlEdTool.h"
#include "UnrealWorldControlEdToolCustomization.h"
#include "Engine/Selection.h"

#include "PropertyEditorModule.h"
#include "SlateOptMacros.h"

#include "IDetailsView.h"
#include "IIntroTutorials.h"

#include "Widgets/Input/SButton.h"
#include "Widgets/Text/STextBlock.h"
#include "EditorModeManager.h"

#define LOCTEXT_NAMESPACE "FUnrealWorldControlEdTool"


void FUnrealWorldControlEdModeToolkit::Init(const TSharedPtr<IToolkitHost>& InitToolkitHost)
{
	FUnrealWorldControlEdMode* UnrealWorldControlEdMode = GetEditorMode();

	UnrealWorldControlEdWidgets = SNew(SUnrealWorldControlEd, SharedThis(this));

	FModeToolkit::Init(InitToolkitHost);
}

FName FUnrealWorldControlEdModeToolkit::GetToolkitFName() const
{
	return FName("UnrealWorldControlEd");
}

FText FUnrealWorldControlEdModeToolkit::GetBaseToolkitName() const
{
	return LOCTEXT("Toolkitname", "New Editor");
}

FUnrealWorldControlEdMode* FUnrealWorldControlEdModeToolkit::GetEditorMode() const
{
	return (FUnrealWorldControlEdMode*)GLevelEditorModeTools().GetActiveMode(
		FUnrealWorldControlEdMode::EM_UnrealWorldControlEdModeId);
}

TSharedPtr<SWidget> FUnrealWorldControlEdModeToolkit::GetInlineContent() const
{
	return UnrealWorldControlEdWidgets;
}

//////////////////////////////////////////////////////////////////////////

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SUnrealWorldControlEd::Construct(const FArguments& InArgs,
                                      TSharedRef<FUnrealWorldControlEdModeToolkit> InParentToolkit)
{
	FPropertyEditorModule& PropertyEditorModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>(
		"PropertyEditor");
	PropertyEditorModule.RegisterCustomClassLayout("UnrealWorldControlEdTool",
	                                               FOnGetDetailCustomizationInstance::CreateStatic(
		                                               &FUnrealWorldControlEdToolCustomization::MakeInstance));
	PropertyEditorModule.NotifyCustomizationModuleChanged();

	FDetailsViewArgs DetailsViewArgs(false, false, false, FDetailsViewArgs::HideNameArea);

	DetailsPanel = PropertyEditorModule.CreateDetailView(DetailsViewArgs);
	DetailsPanel->SetIsPropertyVisibleDelegate(
		FIsPropertyVisible::CreateSP(this, &SUnrealWorldControlEd::GetIsPropertyVisible));

	FUnrealWorldControlEdMode* UnrealWorldControlEdMode = GetEditorMode();

	ChildSlot
	[
		SNew(SVerticalBox)
		+ SVerticalBox::Slot()
		.Padding(0)
		[
			SNew(SVerticalBox)
			.IsEnabled(this, &SUnrealWorldControlEd::GetUnrealWorldControlEdIsEnabled)
			+ SVerticalBox::Slot()
			.Padding(0)
			[
				DetailsPanel.ToSharedRef()
			]
		]
	];

	RefreshDetailPanel();
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION

FUnrealWorldControlEdMode* SUnrealWorldControlEd::GetEditorMode() const
{
	return (FUnrealWorldControlEdMode*)GLevelEditorModeTools().GetActiveMode(
		FUnrealWorldControlEdMode::EM_UnrealWorldControlEdModeId);
}

FText SUnrealWorldControlEd::GetErrorText() const
{
	FUnrealWorldControlEdMode* UnrealWorldControlEdMode = GetEditorMode();

	return FText::GetEmpty();
}

bool SUnrealWorldControlEd::GetUnrealWorldControlEdIsEnabled() const
{
	FUnrealWorldControlEdMode* UnrealWorldControlEdMode = GetEditorMode();
	if (UnrealWorldControlEdMode)
	{
		return true;
	}
	return false;
}

bool SUnrealWorldControlEd::GetIsPropertyVisible(const FPropertyAndParent& PropertyAndParent) const
{
	return true;
}

void SUnrealWorldControlEd::RefreshDetailPanel()
{
	FUnrealWorldControlEdMode* UnrealWorldControlEdMode = GetEditorMode();
	if (UnrealWorldControlEdMode)
	{
		// Refresh details panel
		DetailsPanel->SetObject(UnrealWorldControlEdMode->UISettings, true);
	}
}
#undef LOCTEXT_NAMESPACE
