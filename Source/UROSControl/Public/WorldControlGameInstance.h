// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ROSBridgeGameInstance.h"
#include "RWCManager.h"
#include "WorldControlGameInstance.generated.h"

/**
 *
 */
UCLASS()
class UROSCONTROL_API UWorldControlGameInstance : public UROSBridgeGameInstance
{
  GENERATED_BODY()

    virtual void OnStart() override;
    URWCManager* Manager;
 public:

  UPROPERTY(EditAnywhere, Category = "Config")
    FString Namespace = FString("UnrealSim");

  UPROPERTY(EditAnywhere, Category = "Config")
    bool bEnableRWCManager = false;

};
