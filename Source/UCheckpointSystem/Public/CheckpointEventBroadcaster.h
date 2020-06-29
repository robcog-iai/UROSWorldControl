// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "UCheckpointSystem.h"
#include "CheckpointEventBroadcaster.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSetCheckpointEvent, const FString&, Args);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FGetCheckpointEvent, const FString&, Args);
/**
 *
 */
UCLASS()
class UCHECKPOINTSYSTEM_API UCheckpointEventBroadcaster : public UObject
{
	GENERATED_BODY()
public:

          // Broadcast the checkpoint event depending on the command.
    virtual bool Broadcast(FString InCommand, FString InArgs);


    UPROPERTY(BlueprintAssignable, Category = "Checkpoint")
        FSetCheckpointEvent OnSetCheckpointEvent;

    UPROPERTY(BlueprintAssignable, Category = "Checkpoint")
        FGetCheckpointEvent OnGetCheckpointEvent;
};
