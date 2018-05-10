// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "UnrealWorldControlEdMode.h"
#include "UnrealWorldControlEdSlate.h"
#include "Toolkits/ToolkitManager.h"
#include "EditorModeManager.h"

const FEditorModeID FUnrealWorldControlEdMode::EM_UnrealWorldControlEdModeId = TEXT("EM_UnrealWorldControlEdMode");

FUnrealWorldControlEdMode::FUnrealWorldControlEdMode()
{
	UISettings = NewObject<UUnrealWorldControlEdTool>(GetTransientPackage(), TEXT("NewUISettings"), RF_Transactional);
	UISettings->SetParent(this);
}

FUnrealWorldControlEdMode::~FUnrealWorldControlEdMode()
{

}

void FUnrealWorldControlEdMode::AddReferencedObjects(FReferenceCollector & Collector)
{
	// Call parent implementation
	FEdMode::AddReferencedObjects(Collector);

	Collector.AddReferencedObject(UISettings);
}

void FUnrealWorldControlEdMode::Enter()
{
	FEdMode::Enter();
	
	if (!Toolkit.IsValid() && UsesToolkits())
	{
		Toolkit = MakeShareable(new FUnrealWorldControlEdModeToolkit);
		Toolkit->Init(Owner->GetToolkitHost());
	}
}

void FUnrealWorldControlEdMode::Exit()
{
	if (Toolkit.IsValid())
	{
		FToolkitManager::Get().CloseToolkit(Toolkit.ToSharedRef());
		Toolkit.Reset();
	}

	// Call base Exit method to ensure proper cleanup
	FEdMode::Exit();
}

bool FUnrealWorldControlEdMode::UsesToolkits() const
{
	return true;
}



