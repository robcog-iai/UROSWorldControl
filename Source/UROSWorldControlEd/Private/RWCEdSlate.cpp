// Copyright 2018, Institute for Artificial Intelligence - University of Bremen

#include "RWCEdSlate.h"
#include "RWCEdMode.h"
#include "RWCEdTool.h"
#include "RWCEdToolCustomization.h"
#include "Engine/Selection.h"

#include "PropertyEditorModule.h"
#include "SlateOptMacros.h"

#include "IDetailsView.h"
#include "IIntroTutorials.h"

#include "Widgets/Input/SButton.h"
#include "Widgets/Text/STextBlock.h"
#include "EditorModeManager.h"

#define LOCTEXT_NAMESPACE "FRWCEdTool"


void FRWCEdModeToolkit::Init(const TSharedPtr<IToolkitHost>& InitToolkitHost)
{
	FRWCEdMode* RWCEdMode = GetEditorMode();

	RWCEdWidgets = SNew(SRWCEd, SharedThis(this));

	FModeToolkit::Init(InitToolkitHost);
}

FName FRWCEdModeToolkit::GetToolkitFName() const
{
	return FName("RWCEd");
}

FText FRWCEdModeToolkit::GetBaseToolkitName() const
{
	return LOCTEXT("Toolkitname", "New Editor");
}

FRWCEdMode* FRWCEdModeToolkit::GetEditorMode() const
{
	return (FRWCEdMode*)GLevelEditorModeTools().GetActiveMode(
		FRWCEdMode::EM_RWCEdModeId);
}

TSharedPtr<SWidget> FRWCEdModeToolkit::GetInlineContent() const
{
	return RWCEdWidgets;
}

//////////////////////////////////////////////////////////////////////////

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SRWCEd::Construct(const FArguments& InArgs,
                                      TSharedRef<FRWCEdModeToolkit> InParentToolkit)
{
	FPropertyEditorModule& PropertyEditorModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>(
		"PropertyEditor");
	PropertyEditorModule.RegisterCustomClassLayout("RWCEdTool",
	                                               FOnGetDetailCustomizationInstance::CreateStatic(
		                                               &FRWCEdToolCustomization::MakeInstance));
	PropertyEditorModule.NotifyCustomizationModuleChanged();

	FDetailsViewArgs DetailsViewArgs(false, false, false, FDetailsViewArgs::HideNameArea);

	DetailsPanel = PropertyEditorModule.CreateDetailView(DetailsViewArgs);
	DetailsPanel->SetIsPropertyVisibleDelegate(
		FIsPropertyVisible::CreateSP(this, &SRWCEd::GetIsPropertyVisible));

	FRWCEdMode* RWCEdMode = GetEditorMode();

	ChildSlot
	[
		SNew(SVerticalBox)
		+ SVerticalBox::Slot()
		.Padding(0)
		[
			SNew(SVerticalBox)
			.IsEnabled(this, &SRWCEd::GetRWCEdIsEnabled)
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

FRWCEdMode* SRWCEd::GetEditorMode() const
{
	return (FRWCEdMode*)GLevelEditorModeTools().GetActiveMode(
		FRWCEdMode::EM_RWCEdModeId);
}

FText SRWCEd::GetErrorText() const
{
	FRWCEdMode* RWCEdMode = GetEditorMode();

	return FText::GetEmpty();
}

bool SRWCEd::GetRWCEdIsEnabled() const
{
	FRWCEdMode* RWCEdMode = GetEditorMode();
	if (RWCEdMode)
	{
		return true;
	}
	return false;
}

bool SRWCEd::GetIsPropertyVisible(const FPropertyAndParent& PropertyAndParent) const
{
	return true;
}

void SRWCEd::RefreshDetailPanel()
{
	FRWCEdMode* RWCEdMode = GetEditorMode();
	if (RWCEdMode)
	{
		// Refresh details panel
		DetailsPanel->SetObject(RWCEdMode->UISettings, true);
	}
}
#undef LOCTEXT_NAMESPACE
