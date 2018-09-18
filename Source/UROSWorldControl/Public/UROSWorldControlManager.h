// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "Engine.h"
#include "GameFramework/Actor.h"
#include "Tags.h"
#include "UROSCallbackRegisterBase.h"
#include "UROSWorldControlManager.generated.h"


UCLASS()
class UROSWORLDCONTROL_API UROSWorldControlManager : public UROSCallbackRegisterBase
{
	GENERATED_BODY()

public:
	UROSWorldControlManager(){}
	~UROSWorldControlManager(){}

	void SetupServiceServers();
					
	void Register(FString DefaultNamespace) override;														
													
	TMap<FString, AActor*> IdToActorMap;
private:
	FString Namespace;
};
