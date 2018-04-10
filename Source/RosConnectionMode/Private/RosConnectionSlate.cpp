// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "RosConnectionSlate.h"
#include "RosConnectionMode.h"
#include "RosConnectionTool.h"
#include "RosConnectionToolCustomization.h"
#include "Engine/Selection.h"

#include "PropertyEditorModule.h"
#include "SlateOptMacros.h"

#include "IDetailsView.h"
#include "IIntroTutorials.h"

#include "Widgets/Input/SButton.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Notifications/SErrorText.h"
#include "EditorModeManager.h"

#define LOCTEXT_NAMESPACE "FRosConnectionTool"


void FRosConnectionModeToolkit::Init(const TSharedPtr<IToolkitHost>& InitToolkitHost)
{
	FRosConnectionMode* RosConnectionMode = GetEditorMode();

	RosConnectionWidgets = SNew(SRosConnection, SharedThis(this));

	FModeToolkit::Init(InitToolkitHost);
}

FName FRosConnectionModeToolkit::GetToolkitFName() const
{
	return FName("RosConnection");
}

FText FRosConnectionModeToolkit::GetBaseToolkitName() const
{
	return LOCTEXT("Toolkitname", "New Editor");
}

FRosConnectionMode* FRosConnectionModeToolkit::GetEditorMode() const
{
	return (FRosConnectionMode*)GLevelEditorModeTools().GetActiveMode(FRosConnectionMode::EM_RosConnectionModeId);
}

TSharedPtr<SWidget> FRosConnectionModeToolkit::GetInlineContent() const
{
	return RosConnectionWidgets;
}

//////////////////////////////////////////////////////////////////////////

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SRosConnection::Construct(const FArguments& InArgs, TSharedRef<FRosConnectionModeToolkit> InParentToolkit)
{
	FPropertyEditorModule& PropertyEditorModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
	PropertyEditorModule.RegisterCustomClassLayout("RosConnectionTool", FOnGetDetailCustomizationInstance::CreateStatic(&FRosConnectionToolCustomization::MakeInstance));
	PropertyEditorModule.NotifyCustomizationModuleChanged();

	FDetailsViewArgs DetailsViewArgs(false, false, false, FDetailsViewArgs::HideNameArea);
	
	DetailsPanel = PropertyEditorModule.CreateDetailView(DetailsViewArgs);
	DetailsPanel->SetIsPropertyVisibleDelegate(FIsPropertyVisible::CreateSP(this, &SRosConnection::GetIsPropertyVisible));
	
	FRosConnectionMode* RosConnectionMode = GetEditorMode();
	
	ChildSlot
	[
		SNew(SVerticalBox)
		+ SVerticalBox::Slot()
		.Padding(0)
		[
			SNew(SVerticalBox)
			.IsEnabled(this, &SRosConnection::GetRosConnectionIsEnabled)
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

FRosConnectionMode* SRosConnection::GetEditorMode() const
{
	return (FRosConnectionMode*)GLevelEditorModeTools().GetActiveMode(FRosConnectionMode::EM_RosConnectionModeId);
}

FText SRosConnection::GetErrorText() const
{
	FRosConnectionMode* RosConnectionMode = GetEditorMode();

	return FText::GetEmpty();
}

bool SRosConnection::GetRosConnectionIsEnabled() const
{
	FRosConnectionMode* RosConnectionMode = GetEditorMode();
	if (RosConnectionMode)
	{
		return true;
	}
	return false;
}

bool SRosConnection::GetIsPropertyVisible(const FPropertyAndParent & PropertyAndParent) const
{
	return true;
}

void SRosConnection::RefreshDetailPanel()
{
	FRosConnectionMode* RosConnectionMode = GetEditorMode();
	if (RosConnectionMode)
	{
		// Refresh details panel
		DetailsPanel->SetObject(RosConnectionMode->UISettings, true);
	}
}
#undef LOCTEXT_NAMESPACE