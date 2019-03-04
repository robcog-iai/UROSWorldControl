// Copyright 2018, Institute for Artificial Intelligence - University of Bremen

#pragma once

#include "CoreMinimal.h"
#include "Engine/World.h"
#include "ROSBridgeSrvServer.h"
#include "ROSBridgeHandler.h"
#include "SetPhysicsProperties.h"
#include "RWCManager.h"
#include "Engine/StaticMesh.h"


class FROSSetPhysicsPropertiesServer : public FROSBridgeSrvServer
{
protected:


	FROSSetPhysicsPropertiesServer()
	{
	};

	UWorld* World;

	FThreadSafeBool ServiceSuccess;

	
public:
	URWCManager* Controller;

	FROSSetPhysicsPropertiesServer(FString Namespace, FString Name, UWorld* InWorld, URWCManager* InController) :
		FROSBridgeSrvServer(Namespace + TEXT("/") + Name, TEXT("world_control_msgs/SetPhysicsProperties"))
	{
		Controller = InController;
		World = InWorld;
	}


	TSharedPtr<FROSBridgeSrv::SrvRequest> FromJson(TSharedPtr<FJsonObject> JsonObject) const override;

	TSharedPtr<FROSBridgeSrv::SrvResponse> Callback(TSharedPtr<FROSBridgeSrv::SrvRequest> Request) override;

};
