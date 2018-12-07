// Copyright 2018, Institute for Artificial Intelligence - University of Bremen

#include "RWCEdMode.h"
#include "RWCEdSlate.h"
#include "Toolkits/ToolkitManager.h"
#include "EditorModeManager.h"

const FEditorModeID FRWCEdMode::EM_RWCEdModeId = TEXT("EM_RWCEdMode");

FRWCEdMode::FRWCEdMode()
{
	UISettings = NewObject<URWCEdTool>(GetTransientPackage(), TEXT("NewUISettings"), RF_Transactional);
	UISettings->SetParent(this);
}

FRWCEdMode::~FRWCEdMode()
{
}

void FRWCEdMode::AddReferencedObjects(FReferenceCollector& Collector)
{
	// Call parent implementation
	FEdMode::AddReferencedObjects(Collector);

	Collector.AddReferencedObject(UISettings);
}

void FRWCEdMode::Enter()
{
	FEdMode::Enter();

	if (!Toolkit.IsValid() && UsesToolkits())
	{
		Toolkit = MakeShareable(new FRWCEdModeToolkit);
		Toolkit->Init(Owner->GetToolkitHost());
	}
}

void FRWCEdMode::Exit()
{
	if (Toolkit.IsValid())
	{
		FToolkitManager::Get().CloseToolkit(Toolkit.ToSharedRef());
		Toolkit.Reset();
	}

	// Call base Exit method to ensure proper cleanup
	FEdMode::Exit();
}

bool FRWCEdMode::UsesToolkits() const
{
	return true;
}
