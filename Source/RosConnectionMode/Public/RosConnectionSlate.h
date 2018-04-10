// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Widgets/SCompoundWidget.h"
#include "RosConnectionMode.h"
#include "Toolkits/IToolkitHost.h"
#include "AssetThumbnail.h"
#include "Toolkits/BaseToolkit.h"
#include "Editor.h"

class IDetailsView;
class SRosConnection;
class SErrorText;
struct FPropertyAndParent;

class FRosConnectionModeToolkit : public FModeToolkit
{
public:	
	/** FModeToolkit interface */
	virtual void Init(const TSharedPtr<IToolkitHost>& InitToolkitHost) override;

	/** IToolkit interface */
	virtual FName GetToolkitFName() const override;
	virtual FText GetBaseToolkitName() const override;
	virtual FRosConnectionMode* GetEditorMode() const override;
	virtual TSharedPtr<class SWidget> GetInlineContent() const override;
	

private:
	/** Geometry tools widget */
	TSharedPtr<SRosConnection> RosConnectionWidgets;
};

/**
* Slate widgets for the New Editor Mode
*/
class SRosConnection : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SRosConnection) {}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs, TSharedRef<FRosConnectionModeToolkit> InParentToolkit);

	void RefreshDetailPanel();
	
protected:

	class FRosConnectionMode* GetEditorMode() const;

	FText GetErrorText() const;

	bool GetRosConnectionIsEnabled() const;

	bool GetIsPropertyVisible(const FPropertyAndParent& PropertyAndParent) const;

	TSharedPtr<SErrorText> Error;

	TSharedPtr<IDetailsView> DetailsPanel;
};
