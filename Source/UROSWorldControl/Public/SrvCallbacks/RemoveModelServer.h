#pragma once
#include "CoreMinimal.h"
#include "ROSBridgeHandler.h"
#include "ROSWorldControlManager.h"
#include "DeleteModel.h"

class FROSRemoveModelServer final : public FROSBridgeSrvServer
{
private:
	FROSRemoveModelServer();
	UWorld* World;
	FThreadSafeBool ServiceSuccess;

	ROSWorldControlManager* Controller;

public:
	FROSRemoveModelServer(FString Namespace, FString Name, UWorld* InWorld,
	                      ROSWorldControlManager* InController) :
		FROSBridgeSrvServer(Namespace + TEXT("/") + Name, TEXT("world_control_msgs/DeleteModel"))
	{
		World = InWorld;
		Controller = InController;
	}

	TSharedPtr<FROSBridgeSrv::SrvRequest> FromJson(TSharedPtr<FJsonObject> JsonObject) const override;

	TSharedPtr<FROSBridgeSrv::SrvResponse> Callback(TSharedPtr<FROSBridgeSrv::SrvRequest> Request) override;
};
