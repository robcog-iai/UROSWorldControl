// Copyright 2018, Institute for Artificial Intelligence - University of Bremen

#pragma once
#include "CoreMinimal.h"
#include "ROSBridgeHandler.h"
#include "RWCManager.h"
#include "DeleteModel.h"

class FROSRemoveModelServer final : public FROSBridgeSrvServer
{
private:
	FROSRemoveModelServer();
	UWorld* World;
	FThreadSafeBool ServiceSuccess;

	URWCManager* Controller;

public:
	FROSRemoveModelServer(FString Namespace, FString Name, UWorld* InWorld,
			URWCManager* InController) :
		FROSBridgeSrvServer(Namespace + TEXT("/") + Name, TEXT("world_control_msgs/DeleteModel"))
	{
		World = InWorld;
		Controller = InController;
	}

	TSharedPtr<FROSBridgeSrv::SrvRequest> FromJson(TSharedPtr<FJsonObject> JsonObject) const override;

	TSharedPtr<FROSBridgeSrv::SrvResponse> Callback(TSharedPtr<FROSBridgeSrv::SrvRequest> Request) override;
};
