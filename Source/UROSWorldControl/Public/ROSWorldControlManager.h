// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Tags.h"
#include "ROSBridgeHandler.h"

//Forward declariation to avoid recursion
class SpawnModelServer;
class SpawnMultipleModelsServer;
class SetModelPoseServer;
class RemoveModelServer;

class UROSWORLDCONTROL_API ROSWorldControlManager
{	
public:	
	// Sets default values for this actor's properties
	ROSWorldControlManager(UWorld* InWorld, FString InServerAdress, int InServerPort, FString InNamespace);
	~ROSWorldControlManager();
	
	FString ServerAdress;
	int ServerPort;
	FString Namespace;
	UWorld* World;

public:	
	void ConnectToROSBridge();
	void DisconnectFromROSBridge();
	bool isConnected();

	TMap<FString, AActor*> IdToActorMap;

private:
	ROSWorldControlManager() {};
	TSharedPtr<FROSBridgeHandler> Handler;

	bool bServicesPulished;
};
