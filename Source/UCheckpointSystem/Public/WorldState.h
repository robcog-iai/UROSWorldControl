// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "Engine/StaticMeshActor.h"
#include "WorldState.generated.h"

/**
 *
 */

USTRUCT()
struct FWorldStateInformation
{
  GENERATED_BODY()

  UPROPERTY(EditAnywhere)
  FTransform Pose;

  UPROPERTY(EditAnywhere)
  FVector LinearVel;

  UPROPERTY(EditAnywhere)
  FVector AngularVel;
};


UCLASS()
class UCHECKPOINTSYSTEM_API UWorldState : public USaveGame
{
	GENERATED_BODY()

 public:
          UPROPERTY(VisibleAnywhere)
          TMap<AStaticMeshActor*, FWorldStateInformation> WorldStateMap;

        void AddActor(AStaticMeshActor* InActor);
};
