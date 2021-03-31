// Copyright 2020, Institute for Artificial Intelligence - University of Bremen
// Author: Michael Neumann

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FUCheckpointSystemModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};

// DECLARE_LOG_CATEGORY_EXTERN(LogCheckpoint, Log, All);
