// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "RosConnectionTool.h"
#include "EdMode.h"

class URosConnectionTool;

class FRosConnectionMode : public FEdMode
{
public:
	URosConnectionTool* UISettings;

	const static FEditorModeID EM_RosConnectionModeId;

	/* Constructor */
	FRosConnectionMode();

	/* Destructor */
	virtual ~FRosConnectionMode();

	/** FGCObject interface */
	virtual void AddReferencedObjects(FReferenceCollector& Collector) override;

	// FEdMode interface
	virtual void Enter() override;
	virtual void Exit() override;
	//virtual void Tick(FEditorViewportClient* ViewportClient, float DeltaTime) override;
	//virtual void Render(const FSceneView* View, FViewport* Viewport, FPrimitiveDrawInterface* PDI) override;
	//virtual void ActorSelectionChangeNotify() override;
	bool UsesToolkits() const override;
	// End of FEdMode interface
};