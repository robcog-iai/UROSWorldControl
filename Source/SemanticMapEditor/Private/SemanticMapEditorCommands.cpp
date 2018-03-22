// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "SemanticMapEditorCommands.h"

#define LOCTEXT_NAMESPACE "FSemanticMapEditorModule"

void FSemanticMapEditorCommands::RegisterCommands()
{
	UI_COMMAND(PluginAction, "SemanticMapEditor", "Execute SemanticMapEditor action", EUserInterfaceActionType::Button, FInputGesture());
}

#undef LOCTEXT_NAMESPACE
