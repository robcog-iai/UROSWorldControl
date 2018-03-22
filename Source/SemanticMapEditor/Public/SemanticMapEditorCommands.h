// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"
#include "SemanticMapEditorStyle.h"

class FSemanticMapEditorCommands : public TCommands<FSemanticMapEditorCommands>
{
public:

	FSemanticMapEditorCommands()
		: TCommands<FSemanticMapEditorCommands>(TEXT("SemanticMapEditor"), NSLOCTEXT("Contexts", "SemanticMapEditor", "SemanticMapEditor Plugin"), NAME_None, FSemanticMapEditorStyle::GetStyleSetName())
	{
	}

	// TCommands<> interface
	virtual void RegisterCommands() override;

public:
	TSharedPtr< FUICommandInfo > PluginAction;
};
