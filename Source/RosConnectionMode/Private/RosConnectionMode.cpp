// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "RosConnectionMode.h"
#include "RosConnectionSlate.h"
#include "Toolkits/ToolkitManager.h"
#include "EditorModeManager.h"

const FEditorModeID FRosConnectionMode::EM_RosConnectionModeId = TEXT("EM_RosConnectionMode");

FRosConnectionMode::FRosConnectionMode()
{
	UISettings = NewObject<URosConnectionTool>(GetTransientPackage(), TEXT("NewUISettings"), RF_Transactional);
	UISettings->SetParent(this);
}

FRosConnectionMode::~FRosConnectionMode()
{

}

void FRosConnectionMode::AddReferencedObjects(FReferenceCollector & Collector)
{
	// Call parent implementation
	FEdMode::AddReferencedObjects(Collector);

	Collector.AddReferencedObject(UISettings);
}

void FRosConnectionMode::Enter()
{
	FEdMode::Enter();
	
	if (!Toolkit.IsValid() && UsesToolkits())
	{
		Toolkit = MakeShareable(new FRosConnectionModeToolkit);
		Toolkit->Init(Owner->GetToolkitHost());
	}
}

void FRosConnectionMode::Exit()
{
	if (Toolkit.IsValid())
	{
		FToolkitManager::Get().CloseToolkit(Toolkit.ToSharedRef());
		Toolkit.Reset();
	}

	// Call base Exit method to ensure proper cleanup
	FEdMode::Exit();
}

bool FRosConnectionMode::UsesToolkits() const
{
	return true;
}



