// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "SemanticMapEditorCommands.h"

#define LOCTEXT_NAMESPACE "FSemanticMapEditorModule"

void FSemanticMapEditorCommands::RegisterCommands()
{
	UI_COMMAND(PluginAction, "Connect to ROSBridge", "Connects to the ROSBridge Server. Serveradress and port can be configured in the properties menu of the ROSWorldControlManager.", EUserInterfaceActionType::Button, FInputGesture());
}

#undef LOCTEXT_NAMESPACE
