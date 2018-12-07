// Copyright 2018, Institute for Artificial Intelligence - University of Bremen

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Tags.h"
#include "ROSBridgeHandler.h"

// Forward declarations to avoid recursion
class SpawnModelServer;
class SpawnMultipleModelsServer;
class SetModelPoseServer;
class RemoveModelServer;

class UROSWORLDCONTROL_API FRWCManager
{
public:
	// Sets default values for this actor's properties
	FRWCManager(UWorld* InWorld, FString InServerAdress, int32 InServerPort, FString InNamespace);
	~FRWCManager();

	FString ServerAdress;
	int32 ServerPort;
	FString Namespace;
	UWorld* World;

public:
	void ConnectToROSBridge(FROSWebsocketInfoSignature CustomErrorCallbacks, FROSWebsocketInfoSignature CustomConnectedCallbacks);
	void DisconnectFromROSBridge();
	bool IsConnected();

	TMap<FString, AActor*> IdToActorMap;

private:
	TSharedPtr<FROSBridgeHandler> Handler;
	
	bool bServicesPulished;
};
