// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UnrealWorldControlEdTool.h"
#include "EdMode.h"

class UUnrealWorldControlEdTool;

class FUnrealWorldControlEdMode : public FEdMode
{
public:
	UUnrealWorldControlEdTool* UISettings;

	const static FEditorModeID EM_UnrealWorldControlEdModeId;

	/* Constructor */
	FUnrealWorldControlEdMode();

	/* Destructor */
	virtual ~FUnrealWorldControlEdMode();

	/** FGCObject interface */
	void AddReferencedObjects(FReferenceCollector& Collector) override;

	// FEdMode interface
	void Enter() override;
	void Exit() override;
	//virtual void Tick(FEditorViewportClient* ViewportClient, float DeltaTime) override;
	//virtual void Render(const FSceneView* View, FViewport* Viewport, FPrimitiveDrawInterface* PDI) override;
	//virtual void ActorSelectionChangeNotify() override;
	bool UsesToolkits() const override;
	// End of FEdMode interface
};
