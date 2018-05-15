// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Widgets/SCompoundWidget.h"
#include "UnrealWorldControlEdMode.h"
#include "Toolkits/IToolkitHost.h"
#include "AssetThumbnail.h"
#include "Toolkits/BaseToolkit.h"
#include "Editor.h"

class IDetailsView;
class SUnrealWorldControlEd;
class SErrorText;
struct FPropertyAndParent;

class FUnrealWorldControlEdModeToolkit : public FModeToolkit
{
public:
	/** FModeToolkit interface */
	void Init(const TSharedPtr<IToolkitHost>& InitToolkitHost) override;

	/** IToolkit interface */
	FName GetToolkitFName() const override;
	FText GetBaseToolkitName() const override;
	FUnrealWorldControlEdMode* GetEditorMode() const override;
	TSharedPtr<class SWidget> GetInlineContent() const override;


private:
	/** Geometry tools widget */
	TSharedPtr<SUnrealWorldControlEd> UnrealWorldControlEdWidgets;
};

/**
* Slate widgets for the New Editor Mode
*/
class SUnrealWorldControlEd : public SCompoundWidget
{
public:
SLATE_BEGIN_ARGS(SUnrealWorldControlEd)
		{
		}

	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs, TSharedRef<FUnrealWorldControlEdModeToolkit> InParentToolkit);

	void RefreshDetailPanel();

protected:

	class FUnrealWorldControlEdMode* GetEditorMode() const;

	FText GetErrorText() const;

	bool GetUnrealWorldControlEdIsEnabled() const;

	bool GetIsPropertyVisible(const FPropertyAndParent& PropertyAndParent) const;

	TSharedPtr<SErrorText> Error;

	TSharedPtr<IDetailsView> DetailsPanel;
};
