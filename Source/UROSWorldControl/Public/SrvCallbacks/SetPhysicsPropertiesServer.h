#pragma once

#include "CoreMinimal.h"
#include "Engine/World.h"
#include "ROSBridgeSrvServer.h"
#include "ROSBridgeHandler.h"
#include "SetPhysicsProperties.h"
#include "UROSWorldControlManager.h"
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
	UROSWorldControlManager * Controller;

	FROSSetPhysicsPropertiesServer(FString Namespace, FString Name, UWorld* InWorld, UROSWorldControlManager* InController) :
		FROSBridgeSrvServer(Namespace + TEXT("/") + Name, TEXT("world_control_msgs/SetPhysicsProperties"))
	{
		Controller = InController;
		World = InWorld;
	}


	TSharedPtr<FROSBridgeSrv::SrvRequest> FromJson(TSharedPtr<FJsonObject> JsonObject) const override;

	TSharedPtr<FROSBridgeSrv::SrvResponse> Callback(TSharedPtr<FROSBridgeSrv::SrvRequest> Request) override;

};
