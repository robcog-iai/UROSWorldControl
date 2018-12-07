// Copyright 2018, Institute for Artificial Intelligence - University of Bremen

#pragma once

#include "CoreMinimal.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Widgets/SCompoundWidget.h"
#include "RWCEdMode.h"
#include "Toolkits/IToolkitHost.h"
#include "AssetThumbnail.h"
#include "Toolkits/BaseToolkit.h"
#include "Editor.h"

class IDetailsView;
class SRWCEd;
class SErrorText;
struct FPropertyAndParent;

class FRWCEdModeToolkit : public FModeToolkit
{
public:
	/** FModeToolkit interface */
	void Init(const TSharedPtr<IToolkitHost>& InitToolkitHost) override;

	/** IToolkit interface */
	FName GetToolkitFName() const override;
	FText GetBaseToolkitName() const override;
	FRWCEdMode* GetEditorMode() const override;
	TSharedPtr<class SWidget> GetInlineContent() const override;


private:
	/** Geometry tools widget */
	TSharedPtr<SRWCEd> RWCEdWidgets;
};

/**
* Slate widgets for the New Editor Mode
*/
class SRWCEd : public SCompoundWidget
{
public:
SLATE_BEGIN_ARGS(SRWCEd)
		{
		}

	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs, TSharedRef<FRWCEdModeToolkit> InParentToolkit);

	void RefreshDetailPanel();

protected:

	class FRWCEdMode* GetEditorMode() const;

	FText GetErrorText() const;

	bool GetRWCEdIsEnabled() const;

	bool GetIsPropertyVisible(const FPropertyAndParent& PropertyAndParent) const;

	TSharedPtr<SErrorText> Error;

	TSharedPtr<IDetailsView> DetailsPanel;
};
