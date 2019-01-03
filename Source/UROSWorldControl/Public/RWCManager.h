// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "Engine.h"
#include "GameFramework/Actor.h"
#include "Tags.h"
#include "UROSCallbackRegisterBase.h"
#include "RWCManager.generated.h"


UCLASS()
class UROSWORLDCONTROL_API URWCManager : public UROSCallbackRegisterBase
{
	GENERATED_BODY()

public:
	URWCManager(){}
	~URWCManager(){}

	void SetupServiceServers();
					
	void Register(FString DefaultNamespace) override;														
													
	TMap<FString, AActor*> IdToActorMap;
private:
	FString Namespace;
};
